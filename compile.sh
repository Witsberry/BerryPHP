#!/bin/bash
# BerryPHP/compile.sh
# Compiles berryrt for development.
# Author: Rikaz Zarook, Witsberry

# Install Autotools dependencies
sudo apt-get install -y autoconf automake libtool

cd berryrt
autoreconf -fi
phpize
./configure --with-php-config=$(which php-config)
make clean
make -j$(nproc)
sudo make install
cd ..

echo "BerryRT compiled and installed."