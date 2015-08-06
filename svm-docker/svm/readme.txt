//******  This program is for libSVM training & testing ******
//content: main.cpp svm.cpp svm.h grid.py
//dependencies: install two libraries : (1)opencv 
//              (2)libsvm, replace grid.py in libsvm & edit the path of grid.py in line 839: "/home/cks/libsvm-3.20/tools/grid.py ";
//compile: g++ --std=c++11 main.cpp svm.cpp -lopencv_core -o main 
//run: (1)K-folidng; (2)Do_subsample(0=false, 1=true); (3) save_model(0=flase 1=true) 
       (4)feature_filename.txt(filename 1:f1 2:f2 ...); (5)label.txt  (6):typename (7):layername" 
//        ex: ./main 10 1 0 OM_Caffe.txt OM_caffe_label.txt type layer 
//output: "SVM folding result.txt"
//******Author: HTC SE CK 
