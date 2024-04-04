#!/usr/bin/bash

echo Building the app...

# -O3 -- max speed optimization

CFLAGS="-fdiagnostics-color=always -g -O3 -std=c++11 -Wall"
INCLUDE="./include/"
SRC="./src/main.cpp ./src/record.cpp ./src/register.cpp ./src/printers.cpp"
TARGET="./build/main"

mkdir -p ./build

if g++ $CFLAGS -I $INCLUDE $SRC -o $TARGET;
    then echo -e "\033[92mSuccessfully built the app:\033[0m $TARGET"; 
    else echo -e "\033[91mError!\033[0m";
fi
