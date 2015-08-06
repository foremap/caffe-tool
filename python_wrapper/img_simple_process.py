#!/usr/bin/env python
import os
import sys
import time
from optparse import OptionParser
import Image
import ImageOps

def main():
    parser = OptionParser(usage="usage: %prog [options] [Input image folder] [Output image folder]",
                          version="%prog 1.0")
    parser.add_option("-r", "--resize",
                      action="store",
                      dest="images_dim",
                      type="string",
                      help="Canonical 'height,width' dimensions of input images. [default]:Original")

    parser.add_option("-g", "--gray",
                      action="store_true",
                      dest="gray_flag",
                      default=False,
                      help="Decide covert to gray or not. [default]:False")
  
    parser.add_option("-l", "--rotate",
                      action="store",
                      dest="rotation_degree",
                      type="int",
                      help="Rotation x degree. [default]:None")

    parser.add_option("-m", "--mirror",
                      action="store_true",
                      dest="mirror_flag",
                      help="Decide mirror image or not. [default]:False")

    parser.add_option("-a", "--augment",
                      action="store_true",
                      dest="augment_flag",
                      help="Decide do data augment or not. [default]:False")

    parser.add_option("-b", "--batch_size",
                      action="store",
                      dest="batch_size",
                      type="int",
                      default=None,
                      help="Would do mutiply the batch-size each time. [default]:None")

    (options, args) = parser.parse_args()

    if len(args) != 2:
        parser.error("wrong number of arguments")

    print options
    print args

    input_folder = args[0]
    output_folder = args[1]

    if os.path.exists(output_folder):
        print "Output_file already exist. Please remove it"
        print "rm -rf " + output_folder
        raise Exception("Output_file exist!!!")
    else:
        os.makedirs(output_folder)

    all_img_file = os.listdir(input_folder)#list of filename

    # for batch
    batch_size = options.batch_size if options.batch_size is not None else len(all_img_file)
    print 'batch_size : ', batch_size

    count = 0
    for ind in xrange(0, len(all_img_file), batch_size):
        img_file = all_img_file[ind: min(ind+batch_size, len(all_img_file))]
        img_list = [ Image.open(os.path.join(input_folder, img)) for img in img_file ]#generator

        if options.images_dim is not None:
            image_dims = [int(s) for s in options.images_dim.split(',')]
            img_list = resize(img_list, image_dims)
        if options.gray_flag:
            img_list = gray(img_list)

        if options.rotation_degree is not None: 
            img_list = rotate(img_list, options.rotation_degree)

        if options.mirror_flag: 
            img_list = mirror(img_list)

        if options.augment_flag:
            save(img_file, img_list, output_folder)#0
            
            img_list = rotate(img_list, 90)
            save_(img_file, img_list, output_folder,'_90')#90
            
            img_list = [ Image.open(os.path.join(input_folder, img)) for img in img_file ]
            img_list = rotate(img_list, 180)
            save_(img_file, img_list, output_folder,'_180')#180
            
            img_list = [ Image.open(os.path.join(input_folder, img)) for img in img_file ]
            img_list = rotate(img_list, 270)
            save_(img_file, img_list, output_folder,'_270')#270
       
            img_list = [ Image.open(os.path.join(input_folder, img)) for img in img_file ]#mirror
            img_list = mirror(img_list)
            save_(img_file, img_list, output_folder,'_mirror')
       
            img_list = [ Image.open(os.path.join(input_folder, img)) for img in img_file ]#flip
            img_list = flip(img_list)
            save_(img_file, img_list, output_folder,'_flip')
        else: 
            save(img_file, img_list, output_folder)
        count += 1
        sys.stdout.write("Processing : %d / %d  \r" % (count*len(img_file), len(all_img_file)))
        sys.stdout.flush()

def gray(img_list):
    for img in img_list:
        tmp_img = img.convert('L')
        yield tmp_img

def resize(img_list, image_dims):
    for img in img_list:
        tmp_img = img.resize( (image_dims[1], image_dims[0]), Image.BILINEAR )
        yield tmp_img

def rotate(img_list,degree):
    for img in img_list:
        tmp_img = img.rotate(degree,Image.BILINEAR)
        yield tmp_img

def mirror(img_list):
    for img in img_list:
        tmp_img = ImageOps.mirror(img)
        yield tmp_img

def flip(img_list):
    for img in img_list:
        tmp_img = ImageOps.flip(img)
        yield tmp_img
        
def save(img_file, img_list, output_folder):
    for img_f, img in zip(img_file, img_list):
        img.save(os.path.join(output_folder, img_f))

def save_(img_file, img_list, output_folder, postfix):
    for img_f, img in zip(img_file, img_list):
        img_f_spilt = img_f.split(".")
        output_name = img_f_spilt[0] + postfix + '.bmp'
        img.save(os.path.join(output_folder, output_name))

if __name__ == '__main__':
    main()
