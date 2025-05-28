#!/bin/bash

set -e

BUILD_DIR="build"
INSTALL_DIR="$HOME/miniparser_install"

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"
cmake --build . --target install

echo "Installed to $INSTALL_DIR"
