#!/bin/bash
# BerryPHP/build.sh
# Builds BerryPHP and berryrt extension for PHP 8.4.
# Author: Rikaz Zarook, Witsberry

echo "Building BerryPHP for PHP 8.4..."

# Install dependencies
sudo apt-get update
sudo apt-get install -y autoconf automake libtool php8.4-dev libuv1-dev libhttp-parser-dev libnghttp2-dev libwebsockets-dev libmysqlclient-dev libhiredis-dev

# Verify php-config
PHP_CONFIG=$(which php-config)
if [ -z "$PHP_CONFIG" ]; then
    echo "php-config not found. Ensure php8.4-dev is installed."
    exit 1
fi

# Verify zend_fiber.h
ZEND_FIBER_H=$(find /usr/include/php -name zend_fiber.h 2>/dev/null)
if [ -z "$ZEND_FIBER_H" ]; then
    echo "zend_fiber.h not found. Reinstalling php8.4-dev..."
    sudo apt-get install --reinstall php8.4-dev
    ZEND_FIBER_H=$(find /usr/include/php -name zend_fiber.h 2>/dev/null)
    if [ -z "$ZEND_FIBER_H" ]; then
        echo "Error: zend_fiber.h still not found. Check php8.4-dev installation."
        exit 1
    fi
fi

# Create m4 directory
mkdir -p berryrt/m4

# Build berryrt
cd berryrt
autoreconf -fi
phpize
./configure --with-php-config="$PHP_CONFIG" CFLAGS="-I/usr/include/php/20240829 -I/usr/include/php/20240829/Zend -I/usr/include/php/20240924 -I/usr/include/php/20240924/Zend" LDFLAGS="-L/usr/lib"
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