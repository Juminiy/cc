#!bin/bash

sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get -y autoremove
sudp apt-get -y purge
sudo apt-get -y install gcc g++ gdb make
sudo snap install go --classic
sudo apt-get -y install strace