#!/bin/bash

mkdir -p thirdparty/
cd thirdparty/
git clone https://github.com/axboe/liburing.git
cd liburing
./configure --cc=gcc --cxx=g++
make -j$(nproc)
make liburing.pc
sudo make install