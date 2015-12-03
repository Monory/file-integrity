#! /bin/sh

mkdir -p build
mkdir -p bin
cd build
cmake ..
make
cp src/daemon ../bin/
cp src/client ../bin/
