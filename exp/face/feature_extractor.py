#!/usr/bin/env python
import os, sys
import random
import Image
import numpy as np
import shutil
import csv
from collections import defaultdict
from scipy.spatial import distance

caffe_root = '/home/sean/Sean/DeepDev/caffe/'
sys.path.insert(0, caffe_root + 'python')
import caffe

dist_type = 'cosine'
dist_map = {
    'euclidean': distance.euclidean,
    'cosine': distance.cosine,
    'hamming': distance.hamming
}

resize_dim = 50

def main(argv):
    if (len(argv) != 4+1):
        raise Exception("Please give enough parameter number !!! \
            1. model, 2. caffemodel 3. retrain feature or not 4. img_dir")
    else:
        MODEL_FILE = argv[1]
        PRETRAINED = argv[2]
        RETRAIN = argv[3]
        img_folder = argv[4]

    net = caffe.Net(MODEL_FILE, PRETRAINED, caffe.TEST)
    print [(k, v.data.shape) for k, v in net.blobs.items()]

    feature_file = os.path.join(PRETRAINED.split('/')[-1].replace('caffemodel', '') + 'lfw_feature.txt')

    print feature_file
    print 'Get face feature...'
    if RETRAIN == 'T':
        get_feature(net, 'pool5', img_folder, feature_file, True)
    else:
        get_feature(net, 'pool5', img_folder, feature_file, False)

# file format => img_name f1_f2_f3.........._fn
def get_feature(net, layer, img_dir, feature_file, retrain):
    if retrain:
        try:
            os.remove(feature_file)
        except OSError:
            pass

    if os.path.isfile(feature_file):
        return
    else:
        pass

    img_list = os.listdir(img_dir)

    count = 0
    for img in img_list:
        face = Image.open(os.path.join(img_dir, img)).resize((resize_dim, resize_dim), Image.BILINEAR).convert('L')
        # Doing normalize, also do in train
        face_arr = np.array(face, order="c") / 256.
        # n x c x h x w
        face_arr = face_arr.reshape(1, 1, face_arr.shape[0], face_arr.shape[1])
        #print face_arr.flags['C_CONTIGUOUS'] 
        label = np.array([1])  # Don't care
        
        net.set_input_arrays(face_arr.astype(np.float32), label.astype(np.float32))
        pred = net.forward()
        
        feature = net.blobs[layer].data[0].flat
        
        # convert to feature format
        with open(feature_file, "a") as myfile:
            myfile.write(img + " " + ','.join(str(e) for e in feature) + "\n")

        count += 1
        sys.stdout.write("Processing : %d / %d  \r" % (count, len(img_list)))
        sys.stdout.flush()

        face.close()

if __name__ == '__main__':
    main(sys.argv)
