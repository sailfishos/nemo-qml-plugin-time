Name:       nemo-qml-plugin-time-qt5

Summary:    Wall clock plugin for Nemo Mobile
Version:    0.1.4
Release:    1
License:    BSD
URL:        https://github.com/sailfishos/nemo-qml-plugin-time
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(timed-qt5)
BuildRequires:  pkgconfig(mce)

%description
%{summary}.

%package tests
Summary:    QML time plugin tests
Requires:   %{name} = %{version}-%{release}
Requires:   mce-tools

%description tests
Tests for QML time plugin

%prep
%setup -q -n %{name}-%{version}

%build

%qmake5 

make %{?_smp_mflags}

%install
%qmake5_install

# org.nemomobile.time legacy import
mkdir -p %{buildroot}%{_libdir}/qt5/qml/org/nemomobile/time/
ln -sf %{_libdir}/qt5/qml/Nemo/Time/libnemotime.so %{buildroot}%{_libdir}/qt5/qml/org/nemomobile/time/
sed 's/Nemo.Time/org.nemomobile.time/' < src/qmldir > %{buildroot}%{_libdir}/qt5/qml/org/nemomobile/time/qmldir

%files
%defattr(-,root,root,-)
%license LICENSE.BSD
%dir %{_libdir}/qt5/qml/Nemo/Time
%{_libdir}/qt5/qml/Nemo/Time/libnemotime.so
%{_libdir}/qt5/qml/Nemo/Time/plugins.qmltypes
%{_libdir}/qt5/qml/Nemo/Time/qmldir

# org.nemomobile.time legacy import
%dir %{_libdir}/qt5/qml/org/nemomobile/time
%{_libdir}/qt5/qml/org/nemomobile/time/libnemotime.so
%{_libdir}/qt5/qml/org/nemomobile/time/qmldir

%files tests
%defattr(-,root,root,-)
/opt/tests/nemo-qml-plugins-qt5/time/*
