#! /bin/sh

mkdir build
cd build
cmake ..
make
cp integrity ..
cd ..
rm -rf build