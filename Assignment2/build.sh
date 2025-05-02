#!/bin/bash
set -ex

cd Assignment2 || exit 1

g++ -std=c++17 -I. -I/usr/include/eigen3 -I./fast-cpp-csv-parser \
    main.cpp gauss_lib.cpp \
    -o gauss_solver

echo "Build successful!"
