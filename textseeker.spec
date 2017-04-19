Name: textseeker
Version: 1.0.2
Release: 1
Summary: Simple text viewer and search tool
License: GPLv3+
URL:     https://gitlab.com/tychosoft/textseeker
Source:  https://pub.cherokeesofidaho.org/tarballs/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
%{?fedora:BuildRequires: qt5-qtbase-devel qt5-linguist}
%{?rhel:BuildRequires: qt5-qtbase-devel qt5-linguist}
%{?suse_version:BuildRequires: libqt5-qtbase-devel libqt5-linguist-devel update-desktop-files}
BuildRequires:  gcc-c++
%{?rhel:Group: Applications/Text}
%{?fedora:Group: Applications/Text}
%{?suse_version:Group:          System/X11/Utilities}

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

%if 0%{?suse_version} > 0
%post
%desktop_database_post

%postun
%desktop_database_postun
%endif

