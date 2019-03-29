#!/bin/bash
source /remote/Xilinx/2018.3/SDK/2018.3/settings64.sh
export CROSS_COMPILE=arm-linux-gnueabihf-
export ARCH=arm
cd dtc 
echo `pwd`
export PATH=`pwd`:$PATH
cd ..
echo `pwd`
cd u-boot-xlnx/tools
PATH=`pwd`:$PATH
cd ../..
export CPATH=:$(pwd)/include-hack
export LIBRARY_PATH=:$(pwd)/lib-hack
export LD_LIBRARY_PATH=$(pwd)/lib-hack/
