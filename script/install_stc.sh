#!/bin/bash
# will not use it anymore

sudo apt install python3-pip
pip3 install meson ninja

mkdir ~/.local/lib ~/.local/include

export LIBRARY_PATH=$LIBRARY_PATH:~/.local/lib
export CPATH=$CPATH:~/.local/include
export CC=gcc

cd ../thirdparty
git clone https://github.com/stclib/STC.git
cd STC

meson setup --buildtype debug build --prefix ~/.local
cd build
ninja
ninja install