#!/bin/sh

out_name="$(basename $(pwd))"

if [ "$1" = "-r" ]; then
    # Build for release

    mkdir -p build
    rm -rf build/release && mkdir build/release

    g++ *.cc \
        -static-libgcc -static-libstdc++ -std=c++17 \
        -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
        -o "build/release/$out_name"

    # Build AppImage
    rm -rf dist
    mkdir dist
    
    appimage-builder --recipe AppImageBuilder.yml --skip-test

    if [ $? -ne 0 ]; then
        rm -rf dist
        exit 1
    else
        mv *.AppImage* dist
        cp *.desktop *.png dist
    fi
elif [ "$1" = "-d" ]; then
    # Build for debug
    mkdir -p build
    rm -rf build/debug && mkdir build/debug

    g++ *.cc \
        -g -std=c++17 \
        -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
        -o "build/debug/$out_name"

    # Example for use as argument to test
    tar -czvf "build/debug/$out_name.tgz" "build/debug/$out_name"
else
    echo "Usage: $0 [-r] [-d]"
    exit 1
fi
