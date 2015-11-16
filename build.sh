#! /bin/sh

mkdir build
cd build
cmake ../src
make
cp integrity ..
cd ..
rm -rf build