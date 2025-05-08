#!/bin/bash
# BerryPHP/compile.sh
# Compiles berryrt for development.
# Author: Rikaz Zarook, Witsberry

cd berryrt
phpize
./configure
make clean
make -j$(nproc)
sudo make install
cd ..

echo "BerryRT compiled and installed."