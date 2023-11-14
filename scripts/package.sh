#!/bin/sh

out_name="$(basename $(pwd))"
version=0.0.1

rm -rf dist
mkdir dist
mkdir dist/{rpm,deb}

# Build RPM

rm -rf "dist/$out_name-$version"
mkdir "dist/$out_name-$version"

cp "build/release/$out_name" "dist/$out_name-$version"
cp "$out_name.png" "dist/$out_name-$version"
cp "$out_name.desktop" "dist/$out_name-$version"

tar -C dist --create --file "dist/$out_name-$version.tar.gz" "$out_name-$version" 

cp "$out_name.spec" ~/rpmbuild/SPECS

rpmlint ~/rpmbuild/SPECS/"$out_name.spec"

if [ $? -ne 0 ]; then
    exit 1
fi

mv "dist/$out_name-$version.tar.gz" ~/rpmbuild/SOURCES

rpmbuild -ba ~/rpmbuild/SPECS/"$out_name.spec"

if [ $? -ne 0 ]; then
    exit 1
fi

mv ~/rpmbuild/RPMS/x86_64/$out_name*.rpm dist/rpm

# ISO
#genisoimage -l -o dist/$out_name.iso \
#    build/* \
#    scripts/install.sh \
#    *.desktop *.png *.AppImage
