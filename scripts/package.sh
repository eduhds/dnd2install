#!/bin/sh

out_name="$(basename $(pwd))"
version=0.0.1
maintainer_name="$(git config --global user.name)"
maintainer_email="$(git config --global user.email)"
changelog_date=$(LANG=en_US.UTF-8 date +'%a %b %d %Y')

mkdir -p dist

if [ "$1" = "--rpm" ]; then
    # Build RPM
    rm -rf dist/rpm 2> /dev/null || true && mkdir dist/rpm
    rpm_src=dist/rpm/$out_name-$version

    mkdir $rpm_src

    cp build/release/$out_name $rpm_src
    cp $out_name.png $rpm_src
    cp $out_name.desktop $rpm_src

    tar -C dist/rpm --create --file dist/rpm/$out_name-$version.tar.gz $out_name-$version

echo "Name:           $out_name
Version:        $version
Release:        1%{?dist}
Summary:       Drag and drop to install

License:        GPL
Source0:        %{name}-%{version}.tar.gz

Requires:       gtk3 webkit2gtk4.0

%description
Drag and drop to install

%prep
%autosetup

%install
rm -rf \$RPM_BUILD_ROOT
mkdir -p \$RPM_BUILD_ROOT/%{_bindir}
mkdir -p \$RPM_BUILD_ROOT/%{_datadir}
mkdir -p \$RPM_BUILD_ROOT/%{_datadir}/{applications,icons}
mkdir -p \$RPM_BUILD_ROOT/%{_datadir}/icons/hicolor
mkdir -p \$RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96
mkdir -p \$RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96/apps
cp %{name} \$RPM_BUILD_ROOT/%{_bindir}
cp %{name}.desktop \$RPM_BUILD_ROOT/%{_datadir}/applications
cp %{name}.png \$RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96/apps

%files
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop 
%{_datadir}/icons/hicolor/96x96/apps/%{name}.png


%changelog
* $changelog_date $maintainer_name <$maintainer_email> - $version
- Fixes and improvements
" > $rpm_src/$out_name.spec

    cp $rpm_src/$out_name.spec ~/rpmbuild/SPECS

    rpmlint ~/rpmbuild/SPECS/$out_name.spec

    if [ $? -ne 0 ]; then
        exit 1
    fi

    mv dist/rpm/$out_name-$version.tar.gz ~/rpmbuild/SOURCES

    rpmbuild -ba ~/rpmbuild/SPECS/$out_name.spec

    if [ $? -ne 0 ]; then
        exit 1
    fi

    mv ~/rpmbuild/RPMS/x86_64/$out_name*.rpm dist/rpm/$out_name-$version.rpm
elif [ "$1" = "--deb" ]; then
    # Build DEB
    rm -rf dist/deb 2> /dev/null || true && mkdir dist/deb
    deb_src=dist/deb/$out_name-$version
    
    mkdir $deb_src
    mkdir $deb_src/{etc,usr}

    mkdir $deb_src/usr/{bin,share}
    mkdir $deb_src/usr/share/{applications,icons,doc}
    mkdir $deb_src/usr/share/doc/$out_name
    mkdir $deb_src/usr/share/icons/hicolor
    mkdir $deb_src/usr/share/icons/hicolor/96x96
    mkdir $deb_src/usr/share/icons/hicolor/96x96/apps

    echo "NAME=$out_name" > $deb_src/etc/$out_name.conf

    strip --strip-debug --strip-unneeded \
        -o $deb_src/usr/bin/$out_name \
        build/release/$out_name

    if [ $? -ne 0 ]; then
        echo "Failed to strip binary"
        exit 1
    fi

    cp $out_name.desktop $deb_src/usr/share/applications
    cp $out_name.png $deb_src/usr/share/icons/hicolor/96x96/apps

    echo "$out_name

Copyright: 2023-$(date +%Y) $maintainer_name <$maintainer_email>

$(date +%Y-%m-%d)

The entire code base may be distributed under the terms of the GNU General
Public License (GPL), which appears immediately below.  Alternatively, all
of the source code as any code derived from that code may instead be
distributed under the GNU Lesser General Public License (LGPL), at the
choice of the distributor. The complete text of the LGPL appears at the
bottom of this file.

See /usr/share/common-licenses/(GPL|LGPL)" > $deb_src/usr/share/doc/$out_name/copyright

    echo "$out_name
* $changelog_date $maintainer_name <$maintainer_email> - $version
- Fixes and improvements" > $deb_src/usr/share/doc/$out_name/changelog

    tar -C $deb_src/usr/share/doc/$out_name \
        -czf $deb_src/usr/share/doc/$out_name/changelog.gz changelog
    rm $deb_src/usr/share/doc/$out_name/changelog

    mkdir $deb_src/DEBIAN
    
    echo "Package: $out_name
Version: $version
Section: utils
Priority: optional
Architecture: amd64
Maintainer: $(git config --global user.name) <$(git config --global user.email)>
Description: A drag and drop installer for Linux
    Install tarballs, .deb or .rpm packages
Homepage: https://github.com/eduhds/dnd2install
Depends: libc6, libgtk-3-0, libwebkit2gtk-4.0-37
Package-Type: deb" > $deb_src/DEBIAN/control

    echo "/etc/$out_name.conf" > $deb_src/DEBIAN/conffiles

    dpkg-deb --root-owner-group --build $deb_src

    dpkg -c dist/deb/$out_name-$version.deb

    lintian dist/deb/$out_name-$version.deb
elif [ "$1" = "--iso" ]; then
    # Build ISO
    if [ -f "dist/deb/$out_name-$version.deb" ] && [ -f "dist/rpm/$out_name-$version.rpm" ]; then
        rm dist/*.iso 2> /dev/null
        
        genisoimage -l -o dist/$out_name-$version.iso \
            dist/deb/$out_name-$version.deb \
            dist/rpm/$out_name-$version.rpm
    else
        echo "Missing packages RPM and/or DEB"
    fi
else
    echo "Unknown package type"
fi
