#!/bin/sh

rm -rf build
mkdir -p build dist

out_name="$(basename $(pwd))"

g++ *.cc \
    -g -static-libgcc -static-libstdc++ -std=c++17 \
    -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
    -o "build/$out_name"

tar -czvf "dist/$out_name.tgz" "build/$out_name"
