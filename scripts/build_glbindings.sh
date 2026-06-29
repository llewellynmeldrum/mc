#!/usr/bin/env bash
# check we are in root dir
cd src/../ 
cd shaders/../ 


rm -rf ./build

cmake -S ./external/glbinding -B build -G "Unix Makefiles" \
    -DCMAKE_CXX_COMPILER=gcc\
    -DCMAKE_C_COMPILER=gcc\
    -DCMAKE_BUILD_TYPE=Release

cmake --build ./external/glbinding/build



