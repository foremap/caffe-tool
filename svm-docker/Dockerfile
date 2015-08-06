# Pull base image.
FROM dockerfile/ubuntu

# Install required package 
RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    libopencv-dev

# Download LIBSVM
RUN wget http://www.csie.ntu.edu.tw/~cjlin/cgi-bin/libsvm.cgi?+http://www.csie.ntu.edu.tw/~cjlin/libsvm -O /opt/libsvm.zip && \
    unzip -d /opt/ /opt/libsvm.zip && \
    rm /opt/libsvm.zip && \
    mv /opt/libsvm-* /opt/libsvm

# Put the svm folder
ADD svm /opt/svm

# Compiler the svm
RUN cp /opt/svm/grid.py /opt/libsvm/tools/
RUN (cd /opt/libsvm && make)
RUN (cd /opt/svm && g++ --std=c++11 main.cpp svm.cpp -lopencv_core -o svm.exe)

# Clean up APT when done.
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*