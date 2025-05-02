#!/bin/bash
set -ex

cd Assignment2 || exit 1

g++ -std=c++17 -I. -I/usr/include/eigen3 -I./fast-cpp-csv-parser \
    gauss_tests.cpp gauss_lib.cpp \
    -lgtest -lgmock -lpthread \
    -o gauss_tests

./gauss_tests

echo "All tests passed!"
