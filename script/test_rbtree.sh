#!/bin/bash

for j in {1..10}
do
    for i in 10 100 1000 10000 100000 1000000
    do
        ./avltree_test.d $i
        if [ $? -eq 1 ]; then
            echo "test fail"
            exit 2
        fi
    done
done 