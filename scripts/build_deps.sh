#!/usr/bin/env bash


if [[ $(hostname) != 'PMs-M1-Pro.local']]; then
    # they arent on my machine
    tput setaf 1
    tput bold
    printf "!!!!WARNING!!!!"
    tput sgr0 0
    printf "\n This project uses a decent amount of ranges and other C++23 (potentially some 26) features which are currently only avaliable in gcc-16. Future versions of clang or msvc will work, but just be sure you are compiling dependencies with the same libc++/libstdc++."
    printf "\n More specifically, you must use the same version of c++ standard library to build these dependencies as you use for the main build. If you compile with libc++, and then try to build with gcc-16, the final link will fail, as gcc will attempt to resolve symbols for clang's stdlib (libc++), and vice versa if you use clang."

    printf "!!!!WARNING 2!!!!"
    tput sgr0 0
    printf "\nYou must configure this build_deps script with the same compiler that you intend to use to build the main files. If you use clang-21 here, and then gcc-16 in the main build, the program will fail at link time (symbols not found)."
fi

CXX="/opt/gcc-16/bin/gcc"
export CXX

# glbinding is the only precompiled library i use at the moment i think
./build_glbinding.sh 
