#!/bin/bash
sudo docker build -t svm-service .
sudo docker run -it -v /opt/feature:/opt/feature svm-service bash