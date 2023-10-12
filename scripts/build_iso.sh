#!/bin/sh

rm -rf dist 
mkdir dist

genisoimage -l -o dist/app_installer.iso \
    build/* \
    scripts/install.sh \
    *.desktop *.png \
