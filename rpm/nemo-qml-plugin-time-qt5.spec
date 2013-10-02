# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.26
# 

Name:       nemo-qml-plugin-time-qt5

# >> macros
# << macros

Summary:    Wall clock plugin for Nemo Mobile
Version:    0.0.0
Release:    1
Group:      System/Libraries
License:    BSD
URL:        https://github.com/nemomobile/nemo-qml-plugin-time
Source0:    %{name}-%{version}.tar.bz2
Source100:  nemo-qml-plugin-time-qt5.yaml
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(timed-qt5)

%description
%{summary}.

%package tests
Summary:    QML time plugin tests
Group:      System/Libraries
Requires:   %{name} = %{version}-%{release}

%description tests
Tests for QML time plugin

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake5 

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake_install

# >> install post
# << install post

%files
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/org/nemomobile/time/libnemotime.so
%{_libdir}/qt5/qml/org/nemomobile/time/qmldir
# >> files
# << files

%files tests
%defattr(-,root,root,-)
/opt/tests/nemo-qml-plugins-qt5/time/*
# >> files tests
# << files tests