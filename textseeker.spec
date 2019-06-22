Name: textseeker
Epoch:   1
Version: 1.3.0
Release: 1
Summary: Simple text viewer and search tool
License: GPL-3.0+
Group:   Applications/Text
Vendor:  Tycho Softworks
URL:     https://gitlab.com/tychosoft/textseeker
Source:  https://cloud.tychosoft.com/package/source/%{name}-%{version}.tar.gz
BuildRequires: libqt5-qtbase-devel libqt5-linguist-devel
BuildRequires:  gcc-c++

%description
A simple text search and viewer utility.  This is somewhat like the
original gnome search tool.

%prep
%setup -q

%build
qmake-qt5 QMAKE_CXXFLAGS+="%optflags" QMAKE_STRIP=%__strip
%{__make} %{?_smp_mflags}

%install
%{__make} install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root)
%doc README.md LICENSE CHANGELOG
%{_bindir}/textseeker
%{_datadir}/applications/%{name}.desktop
%{_datadir}/pixmaps/%{name}.png
%{_datadir}/metainfo/%{name}.appdata.xml
%{_mandir}/man1/%{name}.1*

