Python_wrapper scripts for Caffe
===========================

The python scripts in python_wrapper is to let runing the **Caffe** more easier, simpler and cleaner. Please see the **Example** (check out the [Example](#Example) section) to see how to train the datasets by the caffe model (in *example* folder). It would step-by-step show you how to use these python scripts.    

----------

Scripts
-------------------
####<big>pre-install.sh</big>
This is a shell script for installing some package we need. Please run this first to make sure all of dependencies is installed.

####<big>config.ini</big>
Specify some env we need. Please make sure all of pathes are correct.

####<big>split_src.py</big>
Suppose we have a dataset folder and it's subforlder is each class. This script could help split datasets to train, validate, test, ..., depends on what you need. And you could specify the ratio/number images in each folder.

```
./split_src.py -h
Usage: split_src.py [options] [Input image folder]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -o OUTPUT_DIR, --output_dir=OUTPUT_DIR
                        ratio of each sub folder. [default]:data_out
  -r RATIO_DIM, --ratio=RATIO_DIM
                        ratio/number of each sub folder. [default]:7:3
  -n FOLDER_NAME, --name=FOLDER_NAME
                        Name of sub folder. [default]:7,3 (Same as ratio)
  --number              Not use ratio but fix number. [default]:False
```

####<big>img2leveldb.py</big>
Because caffe only support two different input format, mdb & leveldb, we need convert the image to one of this. For conveience, I select leveldb. The only drawback is leveldb only could be read by one program at time. You could do greyscale, resize when converting. 

```
./img2leveldb.py -h
Usage: img2leveldb.py [options] [Input image folder] [Label file]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -o OUTPUT_FILE, --output_file=OUTPUT_FILE
                        Name of output folder. [default]:out_leveldb
  -d IMAGES_DIM, --images_dim=IMAGES_DIM
                        Canonical 'height,width' dimensions of images.
                        [default]:256,256
  --isgray              Image is gray or not. [default]:False
  --gray                Decide covert to gray or not. [default]:False
  --mean                Create the mean file or not. [default]:False
```

####<big>leveldb2img.py</big>
Covert the leveldb(should be based on the caffe datum) to image. You could scpecify how many number to show image after convert, or specify the save option and it would base on image label to save the jpg and put in the out_put_dir/[label] folder.

```
./leveldb2img.py -h
Usage: leveldb2img.py [options] [Input leveldb folder]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -o OUTPUT_FILE, --output_file=OUTPUT_FILE
                        Name of output folder. [default]:out_image
  -d IMAGES_DIM, --images_dim=IMAGES_DIM
                        Canonical 'height,width' dimensions of images.
                        [default]:Original size
  -n SHOW_NUM, --show_num=SHOW_NUM
                        Only show/save number of image. [default]:5
  --isgray              Image is gray or not. [default]:False
  -s, --save            Save image in output dir or not. [default]:False
```

####<big>merge_leveldb.py</big>
Merge the muti-leveldb to one. Create a folder and put all of leveldb want to be merged in that, and after merge, you would get a new leveldb contains all of them.

```
./merge_leveldb.py -h
Usage: merge_leveldb.py [options] [Input leveldb dir, put all leveldb under this dir] [Output leveldb folder]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  --isgray              Image is gray or not. [default]:False
  -d IMAGES_DIM, --images_dim=IMAGES_DIM
                        Canonical 'height,width' dimensions of images.
                        [default]:Original size
```

####<big>prediction.py</big>
This script is used for classification. Please make sure the option setting is same as your model at pre-process level. The output would give some metrics to tell the model is good or not.

```
./prediction.py -h
/home/sean/Sean/DeepDev/caffe
Usage: prediction.py [options] [Input image folder] [Label file] [Model file(*.prototxt)] [Caffe Train(*.caffemodel)]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -o OUTPUT_FILE, --output_file=OUTPUT_FILE
                        The prediction result file name. [default]:predict_out
  -m MEAN, --mean=MEAN  Data set image mean *.binaryproto.(for preprocessing
                        options) [default]:None
  -c CHANNEL_SWAP, --channel_swap=CHANNEL_SWAP
                        Order to permute input channels. The default converts
                        RGB -> BGR since BGR is the Caffe default by way of
                        OpenCV.(for preprocessing options) [default]:2,1,0
  -d IMAGES_DIM, --images_dim=IMAGES_DIM
                        Canonical 'height,width' dimensions of input images.
                        [default]:Net input size
  -p INPUT_SCALE, --input_scale=INPUT_SCALE
                        Multiply input features by this scale to finish
                        preprocessing. [default]:None
  -r RAW_SCALE, --raw_scale=RAW_SCALE
                        Multiply raw input by this scale before preprocessing.
                        [default]:None
  -g, --gpu             Using gpu or not. [default]:False

```

####<big>img_simple_process.py</big>
Do some simple image process. For now only support resize and greysclae.

```
./img_simple_process.py  -h
Usage: img_simple_process.py [options] [Input image folder] [Output image folder]

Options:
  --version             show program's version number and exit
  -h, --help            show this help message and exit
  -r IMAGES_DIM, --resize=IMAGES_DIM
                        Canonical 'height,width' dimensions of input images.
                        [default]:Original
  -g, --gray            Decide covert to gray or not. [default]:False

```

----------

<a id="Example"></a>Example
-------------------
Please follow Step-by-Step tutorial to show you how to use these scripts.

####1.Download the dataSets:(Please refer [here](http://wang.ist.psu.edu/docs/related/))

After execute the following commend. We would get a 1000 images with 10 classes in image.orig folder (100 pictures per class).

```
./example/download.sh
```

>James Z. Wang, Jia Li, Gio Wiederhold, ``SIMPLIcity: Semantics-sensitive Integrated Matching for Picture LIbraries,'' IEEE Trans. on Pattern Analysis and Machine Intelligence, vol 23, no.9, pp. 947-963, 2001.

####2.Create Training, Test and Validation sets:

This is just a examples, you can split data to many parts as you want. This following cmds split three parts for train, test, validate in exmaple/data/ with the ratio 5:3:2. And would also create a label file for each part.

```
./python_wrapper/split_src.py -o example/data -r 5:3:2 -n train,test,val example/image.orig
```

####3.Covert train & test to leveldb:

For caffe training, we need covert the data in leveldb(or mdb) format. Sometimes we need subtract the mean in image preprocess, this scripts also could help to create the mean binaryproto file. The following cmd would create leveldb and mean in the example/data folder.

- train

```
./python_wrapper/img2leveldb.py -o ./example/data/train_leveldb -d 32,32 --mean ./example/data/train ./example/data/train.txt
```

- test

```
./python_wrapper/img2leveldb.py -o ./example/data/test_leveldb -d 32,32 --mean ./example/data/test ./example/data/test.txt
```

####4.Training the model:

Before trainging the model, we need modify the files in example/model. There are three file need to be modify.

  - **train_val.prototxt**  
    Please modify the data path for both train & test. And you also adjust the output size in each layer, or change to other model based on your request.(See Caffe documents).

  - **solver.prototxt**  
    This is the input of caffe training. Define some training strategies.
       
  - **deploy.prototxt**  
    This is very same as train_val.prototxt. It is used to predict/classification.

After finish training, it would give you a caffemodel file (would be cifar10_quick_net_iter_4000.caffemodel in this sample), which store all prarmeters value, and need to be feed in Classification.

```
./python_wrapper/train_model.py example/model/solver.prototxt
```

####5.Classification:

After training finish, we could use other data to test the accuracy. For classificatio, the most useful and simple info is Confusion_matrix. You could see in each class, how the model classification. In the future, prediction.py would provide more info and write in a output file.

```
./python_wrapper/prediction.py example/data/val example/data/val.txt example/model/deploy.prototxt example/model/cifar10_quick_net_iter_4000.caffemodel -m example/data/train_leveldb_mean.binaryproto -r 255
```