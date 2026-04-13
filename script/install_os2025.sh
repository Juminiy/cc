#!/bin/bash

mkdir -p data

if [ ! -f "data/gpt2_124M.bin" ]; then
    wget https://box.nju.edu.cn/f/ca18466ad6054f2d85c3/?dl=1 -O data/gpt2_124M.bin
fi

cd jyywiki
if [ ! -d "os2025" ]; then
    git clone https://git.nju.edu.cn/jyy/os2025.git
fi
cd os2025
for i in $(seq 1 9); do
    git pull origin "M$i"
done