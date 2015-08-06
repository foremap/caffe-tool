#!/usr/bin/env python 
import os, sys
import ConfigParser
from optparse import OptionParser
import subprocess
from subprocess import call

def read_conf():
    config = ConfigParser.ConfigParser()
    config.read(os.path.join(os.path.dirname(__file__), 'config.ini'))
    global caffe_root
    caffe_root = config.get('Global', 'caffe_home')

def main():
    parser = OptionParser(usage="usage: %prog [options] [Solver]",
                          version="%prog 1.0")
    parser.add_option("-s", "--snapshot",
                      action="store",
                      dest="snapshot",
                      type="string",
                      default="",
                      help="The snapshot solver state to resume training. [default]:''")
    
    parser.add_option("-w", "--weights",
                      action="store",
                      dest="weights",
                      type="string",
                      default="",
                      help="The pretrained weights to initialize finetuning. " +
                        "Cannot be set simultaneously with snapshot. [default]:''")

    parser.add_option("-g", "--gpu",
                      action="store",
                      dest="gpu_index",
                      type="int",
                      default=-1,
                      help="Run in GPU mode on given device ID. [default]:-1 (Use CPU)")

    (options, args) = parser.parse_args()

    if len(args) != 1:
        parser.error("wrong number of arguments")

    cmd = '%s/caffe train --solver=%s --snapshot=%s --weights=%s --gpu=%d' \
            % (os.path.join(caffe_root, 'build/tools'), args[0], options.snapshot, options.weights, options.gpu_index)

    run(cmd, False)

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