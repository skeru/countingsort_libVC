#!/bin/bash

rm -rf build
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX="." -DLLVM_FIND_VERBOSE=1
make VERBOSE=1
make install
