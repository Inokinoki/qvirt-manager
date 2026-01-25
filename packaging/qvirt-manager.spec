Name:           qvirt-manager
Version:        1.0.0
Release:        1%{?dist}
Summary:        Qt-based virtual machine manager

License:        GPLv2+
URL:            https://github.com/virt-manager/qvirt-manager
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  cmake >= 3.16
BuildRequires:  gcc-c++
BuildRequires:  qt5-qtbase-devel
BuildRequires:  qt5-qttools-devel
BuildRequires:  libvirt-devel
BuildRequires:  pkg-config

Requires:       libvirt >= 4.5.0
Requires:       qt5-qtbase >= 5.9.0
Recommends:     qemu-kvm
Recommends:     spice-client-gtk

%description
QVirt-Manager is a Qt-based virtual machine management application.
It provides a graphical interface for managing libvirt VMs and KVM/QEMU
hypervisors. It is a modern replacement for the GTK+ virt-manager.

Features:
- VM lifecycle management (start, stop, reboot, pause)
- Multi-connection support
- Real-time performance monitoring
- VM creation wizard
- Hardware management
- Storage pool management
- Network management
- Snapshot support
- VM cloning

%prep
%autosetup

%build
mkdir -p build
cd build
%cmake -DQT_VERSION_MAJOR=5 ..
%cmake_build

%install
cd build
%cmake_install

%find_lang %{name}

%files
%license LICENSE
%doc README.md INSTALL.md NEWS.md
%{_bindir}/qvirt-manager

%changelog
* Tue Jan 21 2025 Inoki <veyx.shaw@gmail.com> - 1.0.0-1
- Initial release
- Full feature parity with virt-manager
