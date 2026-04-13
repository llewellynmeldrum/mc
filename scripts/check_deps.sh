#!/usr/bin/env bash

found=0
total=1

pkg-config &>/dev/null
if [ $? -eq 127 ]; then
    printf "ERROR: pkg-config binary not found! Install it, or add it to path!"
else
    found=$((found + 1))
    printf "[%s/%s] pkg-config found." "$found", "$total"
fi
