#!/bin/bash
BASEDIR=$(dirname $0)
cd $BASEDIR

#clean old folder
rm -rf image.orig

wget -t0 -c -N http://wang.ist.psu.edu/~jwang/test1.tar
tar -xvf test1.tar

END=9
for i in $(seq 1 $END)
do 
    echo $i
    mkdir "image.orig/$i/"
    mv `ls image.orig/[$i]??.jpg` "image.orig/$i/"
done
echo 0
mkdir "image.orig/0/"
mv `ls image.orig/*.jpg` "image.orig/0/"