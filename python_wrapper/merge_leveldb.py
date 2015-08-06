#!/usr/bin/env python 
import os, sys
import plyvel
import leveldb
import numpy as np
import ConfigParser
import subprocess
from subprocess import call
from optparse import OptionParser
import Image
"""
Please make sure the Input leveldb dir should be like following.

input_dir
-- leveldb_1
-- leveldb_2
-- leveldb_3
  .
"""

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
    parser = OptionParser(usage="usage: %prog [options] " + 
                        "[Input leveldb dir, put all leveldb under this dir] " + 
                        "[Output leveldb folder]",
                          version="%prog 1.0")

    parser.add_option("--isgray",
                      action="store_true",
                      dest="isGray_flag",
                      default=False,
                      help="Image is gray or not. [default]:False")

    parser.add_option("-d", "--images_dim",
                      action="store",
                      dest="images_dim",
                      type="string",
                      help="Canonical 'height,width' dimensions of images. [default]:Original size")

    parser.add_option("--mean",
                      action="store_true",
                      dest="mean_flag",
                      default=False,
                      help="Create the mean file or not. [default]:False")

    (options, args) = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    input_dir = os.path.join(args[0])
    output_dir = os.path.join(args[1])

    if os.path.exists(output_dir):
        print "Output_file already exist. Please remove it"
        print "rm -rf " + output_dir
        raise Exception("Output_file exist!!!")

    if options.images_dim is not None:
        image_dims = [int(s) for s in options.images_dim.split(',')]
    else:
        image_dims = None
    
    merge_levelDB(input_dir, output_dir, image_dims, options.isGray_flag)

    if options.mean_flag:
        # Create the mean file
        mean_file = output_dir + '_mean.binaryproto'
        cmd = '%s/compute_image_mean -backend leveldb %s %s' % (os.path.join(caffe_root, 'build/tools'), 
                  output_dir, mean_file)
        run(cmd, False)

def merge_levelDB(db_dir, new_folder, image_dims, isgray):
    db = plyvel.DB(new_folder, create_if_missing=True, error_if_exists=True, write_buffer_size=268435456)
    wb = db.write_batch()  
    
    dirs = os.listdir(db_dir)
    count = 0
    for levelDB in dirs:
        print levelDB
        h = leveldb.LevelDB(db_dir + "/" +levelDB)
        datum = caffe_pb2.Datum()
        for key_val,ser_str in h.RangeIter():
            datum.ParseFromString(ser_str)
            # do resize
            if image_dims is not None:
                datum = resize_datum(datum, image_dims, isgray)

            wb.put('%08d_%s' % (count, file), datum.SerializeToString())

            count = count + 1
            if count % 1000 == 0:
                # Write batch of images to database
                wb.write()
                del wb
                wb = db.write_batch()
                print 'Processed %i images.' % count

    if count % 1000 != 0:
        # Write last batch of images
        wb.write()
        print 'Processed a total of %i images.' % count
    else:
        print 'Processed a total of %i images.' % count        


def resize_datum(datum, image_dims, is_gray):
    rows = datum.height
    cols = datum.width
    channel = datum.channels
    label = datum.label

    img_pre = np.fromstring(datum.data, dtype=np.uint8)
    img = img_pre.reshape(channel, rows, cols)
    img = img.transpose((1, 2, 0))
    if is_gray:
        img = img[:, :, 0]
    im = Image.fromarray(img)
    im = im.resize((image_dims[1], image_dims[0]), Image.BILINEAR)
    new_img = np.asarray(im, dtype=np.uint8)\
                .reshape(image_dims[0], image_dims[1], channel)\
                .transpose((2, 0, 1))
    new_datum = caffe.io.array_to_datum(new_img, label)
    return new_datum

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


if __name__ == "__main__":
    read_conf()
    main()
