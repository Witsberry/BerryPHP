#!/bin/bash
# BerryPHP/build.sh
# Builds BerryPHP and berryrt extension.
# Author: Rikaz Zarook, Witsberry

echo "Building BerryPHP..."

# Build berryrt
cd berryrt
phpize
./configure
make clean
make -j$(nproc)
sudo make install
cd ..

# Setup berryphp
mkdir -p berryphp/{src,views,lang,tests,packages,vendor}
mkdir -p berryphp-cli
touch berryphp/lang/{en,fr}.json
touch berryphp/views/welcome.berry

echo "Build complete. Run './run.sh berryphp/tests/composer.php' to test."