Name: textseeker
Epoch:   1
Version: 1.1.0
Release: 1
Summary: Simple text viewer and search tool
License: GPLv3+
Group:   Applications/Text
URL:     https://gitlab.com/tychosoft/textseeker
Source:  https://pub.cherokeesofidaho.org/tarballs/%{name}-%{version}.tar.gz
BuildRequires: qt5-qtbase-devel qt5-linguist
BuildRequires:  gcc-c++

%description
A simple text search and viewer utility.  This is somewhat like the
original gnome search tool.

%prep
%setup -q

%build
qmake-qt5 QMAKE_CXXFLAGS+="\"%optflags\"" QMAKE_STRIP="/bin/true"
%{__make} %{?_smp_mflags}

%install
%{__make} install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%doc README.md LICENSE CHANGELOG
%{_bindir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/appdata/%{name}.appdata.xml
%{_datadir}/translations/%{name}_*.qm
%{_mandir}/man1/%{name}.1*

