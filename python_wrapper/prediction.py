#!/usr/bin/env python
from __future__ import division
import os, sys
import shutil
import csv
import ConfigParser

from optparse import OptionParser

import numpy as np
import matplotlib.pyplot as plt

def read_conf():
    config = ConfigParser.ConfigParser()
    config.read(os.path.join(os.path.dirname(__file__), 'config.ini'))
    caffe_root = config.get('Global', 'caffe_home')
    # Import caffe python
    global caffe, caffe_pb2
    sys.path.insert(0, os.path.join(caffe_root, 'python'))
    import caffe
    from caffe.io import caffe_pb2

def main():
    parser = OptionParser(usage="usage: %prog [options] [Input image folder] [Label file] [Model file(*.prototxt)] [Caffe Train(*.caffemodel)]",
                          version="%prog 1.0")
    parser.add_option("-t", "--type",
                      type='choice',
                      action="store",
                      dest="type",
                      choices=['feature', 'predict',],
                      default='predict',
                      help="2 types [feature, predict], which one to do. [default]:predict")

    parser.add_option("-o", "--output_file",
                      action="store",
                      dest="output_file",
                      type="string",
                      help="The prediction result file name. [default]:[type]_out")

    parser.add_option("-m", "--mean",
                      action="store",
                      dest="mean",
                      type="string",
                      help="Data set image mean *.binaryproto.(for preprocessing options) [default]:None")

    parser.add_option("-c", "--channel_swap",
                      action="store",
                      dest="channel_swap",
                      default='2,1,0',
                      type="string",
                      help="Order to permute input channels. The default converts " +
                          "RGB -> BGR since BGR is the Caffe default by way of OpenCV." + 
                          "(for preprocessing options) [default]:2,1,0")

    parser.add_option("-d", "--images_dim",
                      action="store",
                      dest="images_dim",
                      type="string",
                      help="Canonical 'height,width' dimensions of input images. " +
                            "[default]:Net input size")

    parser.add_option("-p", "--input_scale",
                      action="store",
                      dest="input_scale",
                      type="int",
                      help="Multiply input features by this scale to finish preprocessing. " +
                            "[default]:None")

    parser.add_option("-r", "--raw_scale",
                      action="store",
                      dest="raw_scale",
                      type="int",
                      help="Multiply raw input by this scale before preprocessing. " +
                            "[default]:None")

    parser.add_option("-l", "--layer",
                      action="store",
                      dest="layer",
                      type="string",
                      help="For feature toye, need to specify the layer. [default]:None")

    parser.add_option("-g", "--gpu",
                      action="store_true",
                      dest="gpu_flag",
                      default=False,
                      help="Using gpu or not. [default]:False")

    parser.add_option("-b", "--batch_size",
                      action="store",
                      dest="batch_size",
                      type="int",
                      default=1,
                      help="Would do mutiply the batch-size each time. [default]:1")

    (options, args) = parser.parse_args()

    if len(args) != 4:
        parser.error("wrong number of arguments")

    print options
    print args

    input_dir = args[0]
    label_file = args[1]
    model_def = args[2]
    pretrained_model = args[3]
    if options.output_file is not None:
        output_file = options.output_file
    else:
        output_file = options.type + '_out'
    
    if options.images_dim:
        image_dims = [int(s) for s in options.images_dim.split(',')]
    else:
        image_dims = options.images_dim

    if options.channel_swap:
        channel_swap = [int(s) for s in options.channel_swap.split(',')]    

    # handle mean file
    if options.mean is not None:
        blob = caffe.proto.caffe_pb2.BlobProto()
        data = open(options.mean, 'rb').read()
        blob.ParseFromString(data)
        nparray = np.array( caffe.io.blobproto_to_array(blob) ).squeeze()    
    else:
        nparray = options.mean

    # Make classifier
    if options.gpu_flag:
        caffe.set_mode_gpu()

    classifier = caffe.Classifier(model_def, pretrained_model,
            image_dims=image_dims, mean=nparray,
            input_scale=options.input_scale, raw_scale=options.raw_scale,
            channel_swap=channel_swap)

    batch_size = options.batch_size
    print 'Batch_size = ', batch_size 
    if options.type == 'predict':
        y_true_np, y_pred_np, y_positive_prob_np = \
                                   predict(classifier, input_dir, label_file, batch_size)
        evaluation(y_true_np, y_pred_np, y_positive_prob_np)
    elif options.type == 'feature':
        extract(classifier, input_dir, label_file, options.layer, output_file, batch_size)

def extract(net, input_dir, label_file, layer, output, batch_size):
    print [(k, v.data.shape) for k, v in net.blobs.items()]
    print 'The layer shape is:...'
    print net.blobs[layer].data[0].shape

    try:
        os.remove(output)
    except OSError:
        pass

    with open(label_file, 'rb') as f:
        reader = csv.reader(f, delimiter=' ', dtype=float)
        om_data_list = list(reader)

    count = 0
    for item in om_data_list:
        img_file = item[0]
        label = int(item[1])

        input_image = caffe.io.load_image(os.path.join(input_dir, img_file))
        prediction = net.predict([input_image])

        arr = net.blobs[layer].data[0].flat
        tmpS = ''
        # convert to svm format
        with open(output, "a") as myfile:
            for i in range(0, len(arr)):
                tmpS += str(i+1) + ':' + str(arr[i]) + ' '
            myfile.write(img_file + " " + tmpS + "\n")

        count = count + 1
        sys.stdout.write("Processing : %d / %d  \r" % (count, len(om_data_list)))
        sys.stdout.flush()

def predict(net, input_dir, label_file, batch_size):
    with open(label_file, 'rb') as f:
        reader = csv.reader(f, delimiter=' ')
        om_data_list = list(reader)

    y_true = list()
    y_pred = list()
    y_positive_prob = list()

    count = 0
    for ind in xrange(0, len(om_data_list), batch_size):
        batch_f = om_data_list[ind: min(ind+batch_size, len(om_data_list))]
        img_file = [elem[0] for elem in batch_f]
        label = [int(elem[1]) for elem in batch_f]

        y_true.append(label)

        input_image = [ caffe.io.load_image(os.path.join(input_dir, img)) for img in img_file ]
        #plt.imshow(input_image)
        prediction = net.predict(input_image)
        #print 'prediction shape:', prediction[0].shape
        #plt.plot(prediction[0])
        
        #print prediction, prediction[0].argmax(), prediction[0,1]
        y_pred.append(np.argmax(prediction, axis=1))
        y_positive_prob.append(prediction[:,1])

        count = count + 1
        sys.stdout.write("Processing : %d / %d  \r" % (count * batch_size, len(om_data_list)))
        sys.stdout.flush()

    y_true_np = np.array(y_true).flat
    y_pred_np = np.array(y_pred).flat
    y_positive_prob_np = np.array(y_positive_prob).flat
    print y_true_np, y_pred_np
    return y_true_np, y_pred_np, y_positive_prob_np

def evaluation(y_true_np, y_pred_np, y_positive_prob_np):
    # a.confusion_matrix
    from sklearn.metrics import confusion_matrix
    re = confusion_matrix(y_true_np, y_pred_np)
    print '\n---------------------------------------------------------'     
    print 'Confusion_matrix : '
    print re
    if (re.shape == (2, 2)):
        print 'TN: %10d  | FP: %10d \r' % (re[0,0], re[0,1])
        print 'FN: %10d  | TP: %10d \r' % (re[1,0], re[1,1])  
    print '\nSensitivity(Recall) :' + str(re[1,1]/sum(re[1,:]))
    print 'Specificity         :' + str(re[0,0]/sum(re[0,:]))
    print 'Accuracy            :' + str((re[0,0]+re[1,1])/np.sum(re))

    # b.classification_report
    print '\n---------------------------------------------------------'    
    print 'Classification_report :'
    from sklearn.metrics import classification_report
    print classification_report(y_true_np, y_pred_np)

    # # c.roc_auc_score
    # from sklearn import metrics
    # fpr, tpr, thresholds = metrics.roc_curve(y_true_np, y_positive_prob_np, pos_label = 1)
    # #print fpr, tpr, thresholds
    # roc_auc = metrics.auc(fpr, tpr)
    # print "Area under the ROC curve : %f" % roc_auc

    # print '\n---------------------------------------------------------'
    # # Plot ROC curve
    # plt.figure()
    # plt.plot(fpr, tpr, label='ROC curve (area = %0.2f)' % roc_auc)
    # plt.plot([0, 1], [0, 1], 'k--')
    # plt.xlim([0.0, 1.0])
    # plt.ylim([0.0, 1.0])
    # plt.xlabel('False Positive Rate')
    # plt.ylabel('True Positive Rate')
    # plt.title('Receiver operating characteristic example')
    # plt.legend(loc="lower right")
    # plt.draw()
    # if (re.shape == (2, 2)):
    #     print 'Roc_auc_score :'
    #     from sklearn.metrics import roc_auc_score
    #     print 'roc_auc_score = ', roc_auc_score(y_true_np, y_positive_prob_np)

    print '\n---------------------------------------------------------'
    print 'Accuracy :'
    error = np.subtract(y_pred_np, y_true_np)
    toterr = np.count_nonzero(error)
    print toterr, len(error)
    accuracy = 1- toterr/float(len(error))
    print "accuracy = " , accuracy

    #plt.show()

if __name__ == '__main__':
    read_conf()
    main()
