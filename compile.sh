#!/bin/bash
# BerryPHP/compile.sh
# Compiles berryrt for development.
# Author: Rikaz Zarook, Witsberry

# Install dependencies
sudo apt-get install -y autoconf automake libtool php8.4-dev

# Verify php-config
PHP_CONFIG=$(which php-config)
if [ -z "$PHP_CONFIG" ]; then
    echo "php-config not found. Ensure php8.4-dev is installed."
    exit 1
fi

cd berryrt
autoreconf -fi
phpize
./configure --with-php-config="$PHP_CONFIG" CFLAGS="-I/usr/include/php/20240829 -I/usr/include/php/20240829/Zend" LDFLAGS="-L/usr/lib"
make clean
make -j$(nproc)
sudo make install
cd ..

echo "BerryRT compiled and installed."