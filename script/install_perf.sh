#!/bin/bash

cat "/boot/config-`uname -r`" | grep "PERF_EVENT"
sudo apt-get install linux-tools-common
sudo apt-get install linux-tools-`uname -r`

perf_perm=$(cat /proc/sys/kernel/perf_event_paranoid)
if [ "$perf_perm" -eq "4" ]; then
    sudo sh -c " echo -1 > /proc/sys/kernel/perf_event_paranoid"
    sudo sh -c " echo 0 > /proc/sys/kernel/kptr_restrict"
fi