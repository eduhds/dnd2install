#!/bin/sh

rm -rf build
mkdir build

g++ -g -static-libgcc -static-libstdc++ -std=c++17 \
    -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
    *.cc \
    -o build/dnd_installer
