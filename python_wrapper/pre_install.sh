#!/bin/bash
library_list() {
    library='
        python-matplotlib 
        python-sklearn
        python-imaging'
    echo $library
}

for item in $(library_list)
do
   apt-get install $item -y
done