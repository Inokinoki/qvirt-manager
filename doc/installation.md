# QVirt-Manager Installation Guide

## Table of Contents

1. [Requirements](#requirements)
2. [Building from Source](#building-from-source)
3. [Installing from Packages](#installing-from-packages)
4. [Configuration](#configuration)
5. [Verification](#verification)

---

## Requirements

### System Requirements

- **Operating System**: Linux (kernel 3.10+)
- **Architecture**: x86_64, ARM64, ppc64le
- **Memory**: 2 GB RAM minimum, 4 GB recommended
- **Disk**: 100 MB for application, additional for VM storage

### Software Dependencies

**Required:**
- **Qt**: 5.15+ or 6.2+ (Core, Widgets, Network, UiTools, XML)
- **libvirt**: 4.5.0 or later
- **CMake**: 3.16 or later
- **C++ Compiler**: GCC 8+, Clang 10+, or MSVC 2019+

**Optional:**
- **Spice GTK**: 0.35+ (for SPICE console)
- **GtkVnc**: 1.0+ (for VNC console)
- **libosinfo**: 1.0+ (for OS detection)
- **libguestfs**: 1.40+ (for VM inspection)

---

## Building from Source

### Ubuntu/Debian

```bash
# Install dependencies
sudo apt update
sudo apt install \
    cmake \
    qtbase5-dev \
    qttools5-dev \
    libvirt-dev \
    pkg-config \
    build-essential

# Build
git clone https://github.com/qvirt-manager/qvirt-manager.git
cd qvirt-manager
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install
```

### Fedora/RHEL/CentOS

```bash
# Install dependencies
sudo dnf install \
    cmake \
    qt5-qtbase-devel \
    qt5-qttools-devel \
    libvirt-devel \
    gcc-c++ \
    make

# Build
git clone https://github.com/qvirt-manager/qvirt-manager.git
cd qvirt-manager
mkdir build && cd build
cmake3 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install
```

### Arch Linux

```bash
# Install dependencies
sudo pacman -S \
    cmake \
    qt5-base \
    qt5-tools \
    libvirt \
    gcc

# Build
git clone https://github.com/qvirt-manager/qvirt-manager.git
cd qvirt-manager
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install
```

### Build Options

```bash
# Qt 5 (default)
cmake ..

# Qt 6
cmake -DQT_VERSION_MAJOR=6 ..

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Custom install prefix
cmake -DCMAKE_INSTALL_PREFIX=/opt/qvirt-manager ..

# Enable tests
cmake -DENABLE_TESTS=ON ..
```

---

## Installing from Packages

### Flatpak (Recommended)

```bash
# Install Flatpak
sudo apt install flatpak  # Debian/Ubuntu
sudo dnf install flatpak  # Fedora

# Add Flathub repository
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

# Install QVirt-Manager
flatpak install flathub com.virtmanager.QVirt

# Run
flatpak run com.virtmanager.QVirt
```

### AppImage

```bash
# Download
wget https://github.com/qvirt-manager/qvirt-manager/releases/download/v1.0.0/qvirt-manager-1.0.0-linux-x86_64.AppImage

# Make executable
chmod +x qvirt-manager-1.0.0-linux-x86_64.AppImage

# Run
./qvirt-manager-1.0.0-linux-x86_64.AppImage
```

### Building Packages

**RPM (Fedora/RHEL/CentOS):**
```bash
# Install build dependencies
sudo dnf install rpm-build qt5-qtbase-devel qt5-qttools-devel \
    libvirt-devel cmake gcc-c++

# Build SRPM
rpmbuild -ts qvirt-manager-1.0.0-1.src.rpm

# Or build from spec
rpmbuild -ba packaging/rpm/qvirt-manager.spec
```

**DEB (Debian/Ubuntu):**
```bash
# Install build dependencies
sudo apt install build-essential debhelper cmake \
    qtbase5-dev qttools5-dev libvirt-dev

# Build package
dpkg-buildpackage -us -uc -b

# Install
sudo dpkg -i ../qvirt-manager_1.0.0_amd64.deb
```

---

## Configuration

### Initial Setup

1. **Launch QVirt-Manager**
2. **Add a connection**:
   - Click **File → Add Connection**
   - Select hypervisor (QEMU/KVM)
   - Enter connection URI (e.g., `qemu:///system`)
   - Click **Connect**

3. **Configure preferences** (optional):
   - **Edit → Preferences**
   - Set polling intervals
   - Configure console settings
   - Set default storage path

### Permissions

**For local system access:**
```bash
# Add user to libvirt group
sudo usermod -aG libvirt $(whoami)

# Re-login to apply group changes
newgrp libvirt
```

**For policy kit rules:**
```bash
# Create policy kit rule
sudo nano /etc/polkit-1/rules.d/50-qvirt-manager.rules

# Add:
polkit.addRule(function(action, subject) {
    if (action.id == "org.libvirt.unix.manage" &&
        subject.isInGroup("libvirt")) {
        return polkit.Result.YES;
    }
});
```

### Connection URIs

| Method | URI Format | Example |
|--------|-----------|---------|
| Local System | `qemu:///system` | `qemu:///system` |
| Local Session | `qemu:///session` | `qemu:///session` |
| Remote SSH | `qemu+ssh://user@host/system` | `qemu+ssh://root@192.168.1.10/system` |
| Remote TCP | `qemu+tcp://host:port/system` | `qemu+tcp://192.168.1.10:16509/system` |
| Remote TLS | `qemu+tls://host/system` | `qemu+tls://hypervisor.example.com/system` |

---

## Verification

### Check Installation

```bash
# Check version
qvirt-manager --version

# Help
qvirt-manager --help

# List connections
qvirt-manager --list-connections
```

### Test Connection

1. Open QVirt-Manager
2. Add connection: `qemu:///system`
3. Verify connection shows as "Active"
4. Should see list of VMs (if any exist)

### Create Test VM

1. Click **File → New VM**
2. Name: `test-vm`
3. OS Type: Linux → Ubuntu 22.04
4. Memory: 1024 MB
4. Storage: 10 GB
5. Network: Default NAT
6. Click **Finish**

### Verify Functionality

- VM starts successfully
- Console opens (requires spice-gtk or gtk-vnc)
- Can stop/reboot VM
- Hardware can be added/removed
- Snapshots work

---

## Troubleshooting

### Build Errors

**Problem**: CMake can't find Qt

**Solution**:
```bash
# Set Qt path manually
export Qt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5
cmake ..
```

**Problem**: Missing libvirt headers

**Solution**:
```bash
# Install libvirt development package
sudo apt install libvirt-dev  # Debian/Ubuntu
sudo dnf install libvirt-devel  # Fedora
```

### Runtime Errors

**Problem**: Permission denied accessing libvirt

**Solution**:
```bash
# Add user to libvirt group
sudo usermod -aG libvirt $USER

# Check libvirt is running
sudo systemctl status libvirtd
```

**Problem**: VMs not listed

**Solution**:
- Verify connection URI is correct
- Check libvirt logs: `journalctl -u libvirtd`
- Ensure VMs exist: `virsh list --all`

---

## Uninstallation

### Built from Source

```bash
# If installed
sudo make uninstall

# Manual removal
sudo rm /usr/local/bin/qvirt-manager
sudo rm -rf /usr/local/include/qvirt-manager
sudo rm /usr/local/lib/libqvirt-*
```

### Package Installation

**Flatpak:**
```bash
flatpak uninstall com.virtmanager.QVirt
```

**RPM:**
```bash
sudo dnf remove qvirt-manager
```

**DEB:**
```bash
sudo apt remove qvirt-manager
```

---

## Next Steps

After installation:

1. Read the [User Manual](user-manual.md)
2. Configure your first connection
3. Create or import VMs
4. Explore hardware management features
5. Set up snapshots for backup

For additional help, visit:
- **Wiki**: https://github.com/qvirt-manager/qvirt-manager/wiki
- **Issues**: https://github.com/qvirt-manager/qvirt-manager/issues
- **Discussions**: https://github.com/qvirt-manager/qvirt-manager/discussions

---

*QVirt-Manager Installation Guide v1.0.0*
