# QVirt-Manager Spec File for RPM Packaging

Name:           qvirt-manager
Version:        1.0.0
Release:        1%{?dist}
Summary:        Qt-based Virtual Machine Manager

License:        GPLv2+
URL:            https://github.com/qvirt-manager/qvirt-manager
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++ >= 8.0
BuildRequires:  qt5-qtbase-devel >= 5.15.0
BuildRequires:  qt5-qttools-devel
BuildRequires:  libvirt-devel >= 4.5.0
BuildRequires:  pkgconfig

Requires:       qt5-qtbase >= 5.15.0
Requires:       libvirt >= 4.5.0
Recommends:     spice-gtk3
Recommends:     gtk-vnc2
Recommends:     libosinfo

%description
QVirt-Manager is a Qt-based virtual machine manager, providing a
graphical interface for managing libvirt VMs. It is a modern C++
replacement for virt-manager with improved performance and a
cleaner user interface.

Features:
- VM lifecycle management (start, stop, reboot, pause, save)
- VM creation wizard with OS type detection
- Hardware management (add/edit/remove devices)
- Storage pool and volume management
- Virtual network configuration
- VM snapshot management
- Console viewing (VNC and SPICE)
- Multi-connection support

%package devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.

%prep
%autosetup

%build
%cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DQT_VERSION_MAJOR=5
%cmake_build

%install
%cmake_install

# Install desktop file
desktop-file-install --dir=%{buildroot}%{_datadir}/applications \
    packaging/rpm/qvirt-manager.desktop

# Install icons
install -D -m 0644 packaging/icons/qvirt-manager.svg \
    %{buildroot}%{_datadir}/icons/hicolor/scalable/apps/qvirt-manager.svg
for size in 16 32 48 64 128 256; do
    install -D -m 0644 packaging/icons/${size}x${size}/qvirt-manager.png \
        %{buildroot}%{_datadir}/icons/hicolor/${size}x${size}/apps/qvirt-manager.png
done

%check
%ctest

%files
%doc README.md LICENSE
%{_bindir}/qvirt-manager
%{_datadir}/applications/qvirt-manager.desktop
%{_datadir}/icons/hicolor/*/qvirt-manager.*

%files devel
%{_includedir}/qvirt-manager/
%{_libdir}/libqvirt-*.so

%changelog
* %(date "+%%a %%b %%d %%Y") Inoki <veyx.shaw@gmail.com> - 1.0.0-1
- Initial release
- Complete feature parity with virt-manager
