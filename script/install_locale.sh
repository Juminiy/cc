#!/bin/bash

sudo apt-get install language-pack-zh-hans
sudo locale-gen zh_CN
sudo locale-gen zh_CN.UTF-8
sudo update-locale

LANG=zh_CN.utf8 cat __notfile__.txt