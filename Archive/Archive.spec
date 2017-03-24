Name: $$ARCHIVE
Version: $$VERSION
Release: $$RELEASE
Summary: Simple text viewer and search tool
License: GPL-3.0+
Source:         %{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root
BuildRequires:  libqt5-qtbase-devel
BuildRequires:  libqt5-linguist-devel
BuildRequires:  gcc-c++
BuildRequires:  update-desktop-files
Group:          System/X11/Utilities

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

%post
%desktop_database_post

%postun
%desktop_database_postun
