#!/bin/bash
# BerryPHP/run.sh
# Runs BerryPHP scripts with berryrt and JIT.
# Author: Rikaz Zarook, Witsberry

if [ -z "$1" ]; then
    echo "Usage: ./run.sh <script>"
    exit 1
fi

PHP_EXT_DIR=$(php-config --extension-dir)
if [ ! -f "$PHP_EXT_DIR/berryrt.so" ]; then
    echo "BerryRT extension not found. Run ./build.sh first."
    exit 1
fi

php -dextension=berryrt.so -dzend_extension=opcache.so -dopcache.enable=1 -dopcache.jit_buffer_size=100M "$1"