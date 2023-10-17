#!/bin/sh

rm -rf build
mkdir build

g++ *.cc \
    -g -static-libgcc -static-libstdc++ -std=c++17 \
    -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
    -o build/dnd_installer
