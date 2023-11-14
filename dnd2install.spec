Name:           dnd2install
Version:        0.0.1
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
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/%{_bindir}
mkdir -p $RPM_BUILD_ROOT/%{_datadir}
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/{applications,icons}
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96
mkdir -p $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96/apps
cp %{name} $RPM_BUILD_ROOT/%{_bindir}
cp %{name}.desktop $RPM_BUILD_ROOT/%{_datadir}/applications
cp %{name}.png $RPM_BUILD_ROOT/%{_datadir}/icons/hicolor/96x96/apps

%files
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop 
%{_datadir}/icons/hicolor/96x96/apps/%{name}.png


%changelog

