#! /bin/sh

mkdir build
cd build
cmake ..
make
cp src/integrity ..
cd ..
rm -rf build