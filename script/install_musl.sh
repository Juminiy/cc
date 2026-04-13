#!/bin/bash

# dir: 
./configure --prefix=$HOME/musl \
    --exec-prefix=$HOME/musl \
    --syslibdir=$HOME/musl/lib

# 修改编译参数,musl-gcc带上调试信息
vim config.mak # CFLAGS = -ggdb

make && make install

cp $HOME/musl/bin/musl-gcc $HOME/bin/
# compile c/cpp
# CC="musl-gcc -static"
musl-gcc -v