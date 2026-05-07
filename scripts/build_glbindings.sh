#!/usr/bin/env bash
# check we are in root dir
cd src/../ 
cd shaders/../ 


cd external/glbinding

rm -rf ./build

cmake -S . -B build -G "Unix Makefiles" \
  -DCMAKE_CXX_COMPILER=/opt/gcc-16/bin/g++ \
  -DCMAKE_C_COMPILER=/opt/gcc-16/bin/gcc \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build



