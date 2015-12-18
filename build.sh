#! /bin/sh

mkdir -p build
mkdir -p bin
cd build
cmake ..
make
cp src/integrityctl ../bin/
