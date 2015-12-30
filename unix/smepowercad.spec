Name:		smepowercad
#Version:	0.0
Version:	master
Release:	1%{?dist}
Summary:	A CAD for industrial building design

Group:		Applications/Graphics
License:	GPLv3+
URL:		http://wiki.sme-gmbh.com
Source0:	https://github.com/sme-gmbh/%{name}/archive/master.tar.gz

BuildRequires:	qt-devel >= 5.4
Requires:	libQt5Core
Requires:	libQt5Network
Requires:	libQt5Gui
Requires:	libQt5Widgets
Requires:	libQt5PrintSupport
Requires:	libQt5Svg

%description
SME PowerCAD is a collaborative CAD System for 3D design of industrial buildings
and their technical systems. This program requires OpenGL version 4.3 or later.

%global _hardened_build 1
%global debug_package %{nil}

%prep
%setup -q


%build
qmake-qt5
%{__make} %{?_smp_mflags}


%install
%{__install} -Dm 755 -s %{name} %{buildroot}%{_bindir}/%{name}
%{__install} -Dm 644 unix/%{name}.desktop %{buildroot}%{_datadir}/applications/%{name}.desktop
%{__install} -Dm 644 icons/%{name}.png %{buildroot}%{_datadir}/pixmaps/%{name}.png


# %find_lang %{name}
# desktop-file-install HERE! At the moment this is done by make install.


%files
%{_bindir}/*
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png

%doc


%license LICENSE


%changelog
* Wed Dec 30 2015 Peter Diener <diener.git@sme-gmbh.com> - 0.0-1
- Initial version of the package
