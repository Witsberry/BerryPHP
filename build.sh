#!/bin/bash
# BerryPHP/build.sh
# Builds BerryPHP and berryrt extension for PHP 8.4.
# Author: Rikaz Zarook, Witsberry

echo "Building BerryPHP for PHP 8.4..."

# Install Autotools dependencies
sudo apt-get install -y autoconf automake libtool

# Build berryrt
cd berryrt
autoreconf -fi
phpize
./configure --with-php-config=$(which php-config)
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