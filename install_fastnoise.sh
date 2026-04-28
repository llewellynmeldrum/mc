#!/usr/bin/env sh
set -e
rm -rf ./external/FastNoise2/build

ROOT_DIR=$(dirname "$0")
FN_DIR="$ROOT_DIR/external/FastNoise2"
BUILD_DIR="$FN_DIR/build"

cmake -S "$FN_DIR" -B "$BUILD_DIR" \
  -DFASTNOISE2_TOOLS=OFF \
  -DFASTNOISE2_TESTS=OFF \
  -DFASTNOISE2_UTILITY=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_BUILD_TYPE=Release

cmake --build "$BUILD_DIR" --config Release
