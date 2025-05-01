#!/bin/bash

set -e

g++ -std=c++11 -O2 -o ascii85 ascii85.cpp

echo "Build completed."
