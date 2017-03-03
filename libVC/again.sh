#!/bin/bash

rm *.bc
rm *.so
rm *.log
cd ../..
make
make install
cd bin/test
./libVC_test

