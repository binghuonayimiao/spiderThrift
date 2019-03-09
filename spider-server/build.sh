#!/bin/sh

if [ ! -d "build" ];then
    mkdir build
fi

cd build
cmake ..
make -j4
rm -rf *
cd ../src
rm -rf CMakeFiles  cmake_install.cmake Makefile
cd ../redis
rm -rf CMakeFiles  cmake_install.cmake Makefile

