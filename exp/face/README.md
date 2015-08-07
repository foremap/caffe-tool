Face feature extraction
===========================

There are two components in this folder. One is **face-detection + 2d-alignment** by using dlib library. The other is **feature extraction** by using caffe python wrapper.

----------

Folders
-------------------
####<big>caffe</big>
Put the ***.caffemodel** and **deploy prototxt** in this folder for feature extraction.

####<big>dlib-exec</big>
The exec file compiled from dlib examples based on linux (ubuntu). Not sure other system could run or not. These file is for face detection and 2d-alignment.

####<big>samples</big>
Some face images samples in this folder.

Scripts
-------------------
####<big>dlib-2d-align.py</big>
```
Usage: dlib-2d-align.py [batch_size] [src_dir] [dest_dir] [enlarge]

batch_size: How many images want to process at one time.
src_dir: Input images.
dest_dir: Output images.
enlarge: If the picutre is too small, could enalbe enlarge to help detect performance. (T/F)

example:
     ./dlib-2d-align.py 100 samples test F
```

####<big>feature_extractor.py</big>
```
Usage: dlib-2d-align.py [model] [caffemodel] [re-extract] [img_dir]

model: caffe deploy model.(*.prototxt)
caffemodel: caffe weight model.(*.caffemodel)
re-extract: if enable, would delete olde feature file and re-extractor again.(T/F)
img_dir: The image use for feature extraction. (Need to be done detection and 2d-alignment)

example:
     ./feature_extractor.py caffe/Deepface_softmax_val_CASIA.prototxt  caffe/face_siamese_CASIA_data_iter_260000.caffemodel N test/single/
```