#! /bin/sh

mkdir -p build
cd build
cmake ..
make
cp src/integrity ..
cd ..
