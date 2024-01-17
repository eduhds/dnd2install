#!/bin/sh

app_name="$(basename $(pwd))"

if [ "$1" = "-r" ]; then
    # Build for release
    mkdir -p build
    rm -rf build/release && mkdir build/release

    g++ *.cpp -O3 \
        -static-libgcc -static-libstdc++ -std=c++17 \
        -Ilibs/webview $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
        -o "build/release/$app_name"

    if [ $? -ne 0 ]; then
        exit 1
    fi
elif [ "$1" = "-d" ]; then
    # Build for debug
    mkdir -p build
    rm -rf build/debug && mkdir build/debug

    g++ *.cpp \
        -g -std=c++17 \
        $(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0) \
        -ldl \
        -o "build/debug/$app_name"

    if [ $? -ne 0 ]; then
        exit 1
    fi

    # Example for use as argument to test
    mkdir -p build/debug/test
    tar -C build/debug \
        -czvf build/debug/test/$app_name.tgz \
        $app_name > /dev/null
else
    echo "Usage: $0 [-r] [-d]"
    exit 1
fi
