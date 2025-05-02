#!/bin/bash

set -ex

sudo apt-get install -y libgtest-dev libgmock-dev

g++ -std=c++17 -c -I. -I/usr/include/eigen3 -Ifast-cpp-csv-parser gauss_lib.cpp -o gauss_lib.o

g++ -std=c++17 -I. -I/usr/include/eigen3 -Ifast-cpp-csv-parser \
    gauss_tests.cpp gauss_lib.o \
    -L/usr/local/lib -lgtest -lgtest_main -lgmock -lpthread \
    -o gauss_tests

./gauss_tests

echo "All tests passed!"
