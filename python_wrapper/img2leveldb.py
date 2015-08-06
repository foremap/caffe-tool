#!/usr/bin/env python
import numpy as np
import os, sys
import sys
import glob
import time
import subprocess
import ConfigParser
from subprocess import call
from optparse import OptionParser
import Image

def read_conf():
    config = ConfigParser.ConfigParser()
    config.read(os.path.join(os.path.dirname(__file__), 'config.ini'))
    global caffe_root
    caffe_root = config.get('Global', 'caffe_home')
    # Import caffe python
    global caffe, caffe_pb2
    sys.path.insert(0, os.path.join(caffe_root, 'python'))
    import caffe
    from caffe.io import caffe_pb2    

def main():
    parser = OptionParser(usage="usage: %prog [options] [Input image folder] [Label file]",
                          version="%prog 1.0")
    parser.add_option("-o", "--output_file",
                      action="store",
                      dest="output_file",
                      type="string",
                      default="out_leveldb",
                      help="Name of output folder. [default]:out_leveldb")

    parser.add_option("-d", "--images_dim",
                      action="store",
                      dest="images_dim",
                      type="string",
                      default="256,256",
                      help="Canonical 'height,width' dimensions of images. [default]:256,256")

    parser.add_option("--isgray",
                      action="store_true",
                      dest="isGray_flag",
                      default=False,
                      help="Image is gray or not. [default]:False")

    parser.add_option("--gray",
                      action="store_true",
                      dest="gray_flag",
                      default=False,
                      help="Decide covert to gray or not. [default]:False")

    parser.add_option("--mean",
                      action="store_true",
                      dest="mean_flag",
                      default=False,
                      help="Create the mean file or not. [default]:False")

    (options, args) = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    print options
    print args

    input_file = args[0]
    input_label = args[1]
    output_file = options.output_file

    image_dims = [int(s) for s in options.images_dim.split(',')]
    is_gray = '--gray' if options.isGray_flag else ''
    
    if options.mean_flag:
        mean_file = options.output_file + '_mean.binaryproto'

    if os.path.exists(output_file):
        print "Output_file already exist. Please remove it"
        print "==> rm -rf " + output_file
        raise Exception("Output_file exist!!!")

    if options.gray_flag:
        gray_img_folder = os.path.join(input_file, '../' , '_gray')
        gray(input_file, gray_img_folder)

        cmd = 'GLOG_logtostderr=1 %s/convert_imageset --resize_height=%d \
            --resize_width=%d --backend=%s --shuffle --gray \
            %s %s %s' % (os.path.join(caffe_root, 'build/tools'), image_dims[0], image_dims[1], 'leveldb', gray_img_folder + '/', 
                input_label, output_file)
    else:
        cmd = 'GLOG_logtostderr=1 %s/convert_imageset --resize_height=%d \
            --resize_width=%d --backend=%s --shuffle %s \
            %s %s %s' % (os.path.join(caffe_root, 'build/tools'), image_dims[0], image_dims[1], 'leveldb', is_gray,
                input_file + '/', input_label, output_file)        
    
    run(cmd, False)

    if options.mean_flag:
        # Create the mean file
        cmd = '%s/compute_image_mean %s %s --backend=leveldb' % (os.path.join(caffe_root, 'build/tools'), 
                  output_file, mean_file) 
        run(cmd, False)

def gray(input_folder, output_folder):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    imgs = os.listdir(input_folder)
    count = 0
    for img in imgs:
        tmp_img = Image.open(os.path.join(input_folder, img)).convert('L')
        tmp_img.save(os.path.join(output_folder, img))
    
def run(cmd, need_return):
    print "run the cmd => " + cmd
    if need_return:
        p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    else:
        p = subprocess.Popen(cmd, shell=True, stderr=subprocess.PIPE)

    while True:
        out = p.stderr.read(1)
        if out == '' and p.poll() != None:
            break
        if out != '':
            sys.stdout.write(out)
            sys.stdout.flush()
    print '------- run cmd end ------'
    out1, err1 = p.communicate()

    if need_return:
        result = out1.decode()
    else:
        result = "" 

    return result

if __name__ == '__main__':
    read_conf()
    main()
