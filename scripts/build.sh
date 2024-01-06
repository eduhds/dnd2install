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

    # Build AppImage
    #rm -rf AppDir
    #rm -rf dist
    #mkdir dist
    #
    #appimage-builder --recipe AppImageBuilder.yml --skip-test
    #
    #if [ $? -ne 0 ]; then
    #    rm -rf dist
    #    exit 1
    #else
    #    mv *.AppImage* dist
    #    cp *.desktop *.png dist
    #fi
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
    tar -czvf build/debug/$app_name.tgz build/debug/$app_name > /dev/null
else
    echo "Usage: $0 [-r] [-d]"
    exit 1
fi
