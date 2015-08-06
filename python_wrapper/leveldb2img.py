#!/usr/bin/env python
import numpy as np
import os, sys
import time
import shutil
import subprocess
from subprocess import call
from optparse import OptionParser
import ConfigParser

import leveldb
import Image
import matplotlib.pyplot as plt
import scipy.ndimage

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
    parser = OptionParser(usage="usage: %prog [options] [Input leveldb folder]",
                          version="%prog 1.0")
    parser.add_option("-o", "--output_file",
                      action="store",
                      dest="output_file",
                      type="string",
                      default="out_image",
                      help="Name of output folder. [default]:out_image")

    parser.add_option("-d", "--images_dim",
                      action="store",
                      dest="images_dim",
                      type="string",
                      help="Canonical 'height,width' dimensions of images. [default]:Original size")

    parser.add_option("-n", "--show_num",
                      action="store",
                      dest="show_num",
                      type="int",
                      default=5,
                      help="Only show/save number of image. [default]:5")

    parser.add_option("--isgray",
                      action="store_true",
                      dest="isGray_flag",
                      default=False,
                      help="Image is gray or not. [default]:False")

    parser.add_option("-s", "--save",
                      action="store_true",
                      dest="save_flag",
                      default=False,
                      help="Save image in output dir or not. [default]:False")

    (options, args) = parser.parse_args()

    if len(args) != 1:
        parser.error("wrong number of arguments")

    print options
    print args

    input_file = args[0]
    output_file = options.output_file
    if options.images_dim is not None:
        image_dims = [int(s) for s in options.images_dim.split(',')]
    else:
        image_dims = None
    
    vis_db(input_file, output_file, image_dims, options.isGray_flag, options.show_num, options.save_flag)

def vis_db(db_dir, output_dir, image_dims, is_gray, img_num, save_flag):
    if os.path.exists(output_dir):
        print 'The folder already exists, would clean the folder'
        shutil.rmtree(output_dir)

    count = 0
    
    h = leveldb.LevelDB(db_dir)
    datum = caffe_pb2.Datum()
    for key_val,ser_str in h.RangeIter():
        count = count + 1
        
        datum.ParseFromString(ser_str)
        rows = datum.height
        cols = datum.width
        channel = datum.channels
        label = datum.label
        print rows,cols,channel,label
        img_pre = np.fromstring(datum.data, dtype=np.uint8)

        img = img_pre.reshape(channel, rows, cols)
        img = img.transpose((1, 2, 0))

        print img.shape
        # Change to BGR
        if not is_gray:
            img = img[:, :, (2, 1, 0)]
        else:
            img = img[:, :, 0]

        im = Image.fromarray(img)
        # Do resize here
        if image_dims is not None:
            im = im.resize((image_dims[1], image_dims[0]), Image.BILINEAR)

        if (count <= img_num):
            if save_flag:
                real_out_dir = os.path.join(output_dir, str(label))
                mkdir_p(real_out_dir)
                im.save(os.path.join(real_out_dir, str(count) + '.jpg'), quality=100)
            else:
                im.show()
        else:
            break    

def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)
    else:
        pass

if __name__ == '__main__':
    read_conf()
    main()
