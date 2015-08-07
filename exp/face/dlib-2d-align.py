#!/usr/bin/python
import os, sys
import time
import subprocess

src_dir = ''
dest_dir = ''
enlarg_flag = False
dlib_folder = 'dlib-exec'


def check_file(file_name):
    return os.path.isfile(os.path.join(dest_dir, file_name)) 


def main(batch_size):
    imgs = os.listdir(src_dir)
    count = 0
    time_spend = 0

    mkdir_p(os.path.join(dest_dir, 'single'))
    mkdir_p(os.path.join(dest_dir, 'multi'))
    mkdir_p(os.path.join(dest_dir, 'others'))

    for ind in xrange(0, len(imgs), batch_size):
        img_batch = imgs[ind: min(ind+batch_size, len(imgs))]
        img_batch_r = [x for x in img_batch if not check_file(x)]
        if len(img_batch_r) != 0:
            start_time = time.time()
            arg = [dlib_folder + '/shape_predictor_68_face_landmarks.dat', src_dir, dest_dir] + img_batch_r
            if (enlarg_flag):
                cmd = './' + dlib_folder + '/face_landmark_detection_enlarge ' + ' '.join(arg).replace("'", "\\'")
            else:
                cmd = './' + dlib_folder + '/face_landmark_detection_normal ' + ' '.join(arg).replace("'", "\\'")

            run(cmd, False)

            tmp_t = time.time() - start_time
            time_spend += tmp_t
            print("--- %s seconds, Processing %s images, Avg : %s ---" % (tmp_t, len(img_batch_r), tmp_t / len(img_batch_r)))

        count += 1
        sys.stdout.write("Processing : %d / %d  \r" % (count*batch_size, len(imgs)))
        sys.stdout.flush()

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

def mkdir_p(path):
    if not os.path.exists(path):
        os.makedirs(path)
    else:
        pass

if __name__ == '__main__':
    global src_dir, dest_dir
    batch_size = int(sys.argv[1])
    src_dir = sys.argv[2]
    dest_dir = sys.argv[3]
    enlarge = sys.argv[4]
    enlarg_flag = ( enlarge == 'T')
    main(batch_size)
