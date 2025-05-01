#!/bin/bash

set -ex

g++ -std=c++17 -c -I. -I/usr/include/eigen3 -Ifast-cpp-csv-parser gauss_lib.cpp -o gauss_lib.o

g++ -std=c++17 -I. -I/usr/include/eigen3 -Ifast-cpp-csv-parser \
    main.cpp gauss_lib.o \
    -o gauss_solver

echo "Build successful!"
