#!/usr/bin/env python
import os
from random import shuffle
import sys
import shutil
from optparse import OptionParser

"""
Please make sure the Input image folder topology should be like following(Same as imageNet).

input_dir
-- class_1
  -- xxx.jpg/bmp/...
  -- ...
-- class_2
  -- xxx.jpg/bmp/...
  -- ...
-- class_3
  .
"""

def main():
    parser = OptionParser(usage="usage: %prog [options] [Input image folder]",
                          version="%prog 1.0")
    parser.add_option("-o", "--output_dir",
                      action="store",
                      dest="output_dir",
                      type="string",
                      default="data_out",
                      help="ratio of each sub folder. [default]:data_out")

    parser.add_option("-r", "--ratio",
                      action="store",
                      dest="ratio_dim",
                      type="string",
                      default="7:3",
                      help="ratio/number of each sub folder. [default]:7:3")

    parser.add_option("-n", "--name",
                      action="store",
                      dest="folder_name",
                      type="string",
                      help="Name of sub folder. [default]:7,3 (Same as ratio)")

    parser.add_option("--number",
                      action="store_true",
                      dest="num_flag",
                      default=False,
                      help="Not use ratio but fix number. [default]:False")

    (options, args) = parser.parse_args()

    if len(args) != 1:
        parser.error("wrong number of arguments")

    print options
    print args

    input_dir = args[0]
    output_dir = options.output_dir

    if options.folder_name is not None:
        sub_folder = [s for s in options.folder_name.split(',')]
    else:
        sub_folder = [s for s in options.ratio_dim.split(':')]
     
    ratio_dims = [int(s) for s in options.ratio_dim.split(':')]

    if len(sub_folder) != len(ratio_dims):
        raise Exception("The sub_folder num is not correct, should be same as ratio num")
    
    split(input_dir, output_dir, sub_folder, ratio_dims, options.num_flag)

def split(input_dir, output_dir, sub_dir, ratio_dims, is_num):
    for s_dir in sub_dir:
        mkdir_p(os.path.join(output_dir, s_dir))
        # remove origin label file
        try:
            os.remove(os.path.join(output_dir, s_dir + '.txt'))
        except OSError:
            pass

    all_class = os.listdir(input_dir)
    for class_dir in all_class:
        files = os.listdir(input_dir + "/" + class_dir)
        split_files = getRandom(files, ratio_dims, is_num)
        index = -1
        for s in split_files:
            index = index + 1
            for file_name in s:
                shutil.copy(os.path.join(input_dir, class_dir, file_name), 
                            os.path.join(output_dir, sub_dir[index], class_dir + '_' + file_name))
            # Create label file
            file_name = os.path.join(output_dir, sub_dir[index] + '.txt')
            with open(file_name, "a") as myfile:
                for f_name in s:
                    myfile.write(class_dir + '_' + f_name + " " + class_dir + "\n")

def getRandom(file_list, ratio, is_num):
    shuffle(file_list)
    total = len(file_list)
    if is_num:
        apart = ratio
    else:  
        apart = [ int(float(ratio[i])/sum(ratio)*total) for i in range(len(ratio)) ]
    p_idx = 0
    for i in apart:
        idx = min(p_idx + i, total)
        print p_idx,idx
        yield file_list[p_idx:idx]
        p_idx = idx

def mkdir_p(path):
    if os.path.exists(path):
        print 'The folder already exists, would clean the folder'
        shutil.rmtree(path)
    os.makedirs(path)

if __name__ == '__main__':
    main()
