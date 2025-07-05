#!/bin/bash

set -e

PROJECT_DIR="$(pwd)"
BUILD_DIR="$PROJECT_DIR/tmp-cmake-build-release"
EXT_DIR="$PROJECT_DIR/external/VSCode-extension-base"
CLIENT_DIR="$EXT_DIR/client"

CMAKE_FLAGS=""
if [ "$1" == "-d" ]; then
	echo "Debug mode enabled"
  CMAKE_FLAGS="-DDEBUGING=ON"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake $CMAKE_FLAGS ..
make

cd "$EXT_DIR"

[ ! -d node_modules ] && npm install
[ ! -d client/node_modules ] && cd client && npm install && cd ..

npx tsc -b

code $EXT_DIR

cd "$EXT_DIR"
