#!/bin/sh

mkdir -p dist/app_installer
mkdir -p dist/app_installer/{lib,lib64}

cp build/* dist/app_installer

for lib in $(ldd build/app_installer | grep -E -o '/lib.*\.([0-9]{1,})'); do
    cp -L "$lib" "dist/app_installer$lib"

    if [ $? -ne 0 ]; then
      echo "Falha ao copiar lib: $lib"; exit 1
    fi
done
