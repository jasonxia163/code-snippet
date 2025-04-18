#!/bin/bash

# this is my simple build script

mkdir -p build
cd build
cmake ../cpp/CMakeLists.txt -DCMAKE_BUILD_TYPE=Debug
make
cd ..
