#!/bin/bash
# BerryPHP/compile.sh
# Compiles berryrt for development.
# Author: Rikaz Zarook, Witsberry

cd berryrt
phpize
./configure --with-php-config=$(which php-config)
make clean
make -j$(nproc)
sudo make install
cd ..

echo "BerryRT compiled and installed."