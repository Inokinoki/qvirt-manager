# QVirt-Manager Installation Guide

This guide provides detailed instructions for building and installing QVirt-Manager from source on various platforms.

---

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Linux Installation](#linux-installation)
3. [macOS Installation](#macos-installation)
4. [Windows Installation](#windows-installation)
5. [Build Options](#build-options)
6. [Troubleshooting](#troubleshooting)

---

## Prerequisites

### Required Dependencies

| Dependency | Minimum Version | Recommended |
|------------|-----------------|-------------|
| Qt | 5.9 or 6.2 | 5.15+ or 6.5+ |
| libvirt | 4.5.0 | 8.0.0+ |
| CMake | 3.16 | 3.25+ |
| C++ Compiler | C++17 support | Latest |

### Optional Dependencies

| Dependency | Purpose |
|------------|---------|
| Spice GTK | SPICE console viewer |
| GtkVnc | VNC console viewer |
| libosinfo | OS auto-detection in VM wizard |
| libguestfs | VM inspection and modification |

---

## Linux Installation

### Ubuntu/Debian

#### Using Qt 5 (Recommended for Ubuntu 18.04-22.04)

```bash
# Install build dependencies
sudo apt update
sudo apt install -y \
    cmake \
    qtbase5-dev \
    qttools5-dev \
    libqt5uitools5-dev \
    libvirt-dev \
    build-essential

# Clone or extract source
cd qvirt-manager

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install

# Run
qvirt-manager
```

#### Using Qt 6 (Recommended for Ubuntu 23.04+)

```bash
# Install build dependencies
sudo apt update
sudo apt install -y \
    cmake \
    qt6-base-dev \
    qt6-tools-dev \
    libvirt-dev \
    build-essential

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=6 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install

# Run
qvirt-manager
```

### Fedora/RHEL/CentOS

```bash
# Install build dependencies
sudo dnf install -y \
    cmake \
    qt5-qtbase-devel \
    qt5-qttools-devel \
    libvirt-devel \
    gcc-c++

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install

# Run
qvirt-manager
```

### Arch Linux

```bash
# Install build dependencies
sudo pacman -S \
    cmake \
    qt5-base \
    qt5-tools \
    libvirt \
    gcc

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install

# Run
qvirt-manager
```

### openSUSE

```bash
# Install build dependencies
sudo zypper install -y \
    cmake \
    libqt5-qtbase-common-devel \
    libqt5-qttools-devel \
    libvirt-devel \
    gcc-c++

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Install (optional)
sudo make install

# Run
qvirt-manager
```

---

## macOS Installation

### Using Homebrew

```bash
# Install dependencies
brew install cmake qt@5 libvirt

# Add Qt to path (add to ~/.profile or ~/.zprofile)
echo 'export PATH="/usr/local/opt/qt@5/bin:$PATH"' >> ~/.profile
echo 'export CMAKE_PREFIX_PATH="/usr/local/opt/qt@5"' >> ~/.profile
source ~/.profile

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(sysctl -n hw.ncpu)

# Create app bundle (optional)
make package

# Run
open qvirt-manager.app
# or
./qvirt-manager
```

### Using MacPorts

```bash
# Install dependencies
sudo port install cmake qt5-qtbase libvirt

# Configure and build
mkdir build && cd build
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_PREFIX_PATH=/usr/local/opt/qt5 ..
make -j$(sysctl -n hw.ncpu)

# Install (optional)
sudo make install

# Run
./qvirt-manager
```

---

## Windows Installation

### Using MSYS2

```bash
# Install MSYS2 from https://www.msys2.org/

# Update MSYS2
pacman -Syu

# Install dependencies (in MSYS2 MinGW 64-bit shell)
pacman -S mingw-w64-x86_64-cmake \
         mingw-w64-x86_64-qt5 \
         mingw-w64-x86_64-libvirt \
         mingw-w64-x86_64-gcc \
         mingw-w64-x86_64-ninja

# Configure and build
mkdir build && cd build
cmake -G Ninja -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
ninja

# Run
./qvirt-manager.exe
```

### Using vcpkg

```bash
# Install vcpkg from https://github.com/Microsoft/vcpkg

# Install dependencies
./vcpkg install qt5:x64-windows libvirt:x64-windows

# Configure and build
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake \
      -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release

# Run
./Release/qvirt-manager.exe
```

---

## Build Options

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `QT_VERSION_MAJOR` | Auto-detect | Qt version (5 or 6) |
| `CMAKE_BUILD_TYPE` | Debug | Build type (Debug/Release) |
| `CMAKE_INSTALL_PREFIX` | /usr/local | Installation path |

### Example Configurations

#### Debug Build
```bash
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

#### Release Build with Optimizations
```bash
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

#### Custom Install Prefix
```bash
cmake -DQT_VERSION_MAJOR=5 -DCMAKE_INSTALL_PREFIX=/opt/qvirt-manager ..
make -j$(nproc)
sudo make install
```

#### Verbose Build Output
```bash
make VERBOSE=1 -j$(nproc)
```

---

## Troubleshooting

### Qt Not Found

**Problem**: CMake cannot find Qt installation

**Solution**:
```bash
# Set CMAKE_PREFIX_PATH to Qt installation
cmake -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt5 ..
```

### libvirt Not Found

**Problem**: CMake cannot find libvirt

**Solution**:
```bash
# Install libvirt development package
sudo apt install libvirt-dev  # Ubuntu/Debian
sudo dnf install libvirt-devel  # Fedora

# Or set LIBVIRT_* variables manually
cmake -DLIBVIRT_INCLUDE_DIRS=/usr/include/libvirt \
      -DLIBVIRT_LIBRARIES=/usr/lib/x86_64-linux-gnu/libvirt.so ..
```

### Missing Qt Modules

**Problem**: Qt5UiTools or other modules not found

**Solution**:
```bash
# Ubuntu/Debian
sudo apt install libqt5uitools5-dev

# Fedora
sudo dnf install qt5-qttools-devel
```

### Cannot Connect to libvirt

**Problem**: "Failed to connect to hypervisor" error

**Solution**:
```bash
# Add user to libvirt group
sudo usermod -aG libvirt $(whoami)

# Restart libvirt service
sudo systemctl restart libvirtd

# Re-login for group changes to take effect
```

### Build Fails on Older Qt Versions

**Problem**: Compilation errors on Qt 5.9-5.14

**Solution**: The project is designed to work with Qt 5.9+. If you encounter issues:
1. Verify you have Qt 5.9 or later: `qmake --version`
2. Check that all required Qt modules are installed
3. Try with a newer Qt version if possible

### Runtime Errors on Linux

**Problem**: Application crashes or fails to start

**Solution**:
```bash
# Check library dependencies
ldd qvirt-manager | grep "not found"

# Install missing libraries
sudo apt install libqt5core5a libqt5widgets5 libqt5gui5

# Run with debug output
QT_DEBUG_PLUGINS=1 ./qvirt-manager
```

---

## Verification

### Verify Installation

```bash
# Check version
qvirt-manager --version

# List all VMs (requires connection)
qvirt-manager --connect qemu:///system --list-all
```

### Test Connection

```bash
# Start QVirt-Manager and connect to local hypervisor
qvirt-manager

# Or test libvirt connection directly
virsh -c qemu:///system list --all
```

---

## Next Steps

After installation:

1. **Initial Setup**: See [README.md](README.md) for first-time usage
2. **Configuration**: Configuration stored in `~/.config/qvirt-manager/qvirt-manager.conf`
3. **User Manual**: See [doc/user-manual.md](doc/user-manual.md) for detailed usage
4. **Reporting Issues**: File bugs at GitHub Issues

---

## Additional Resources

- [libvirt Documentation](https://libvirt.org/docs.html)
- [Qt Documentation](https://doc.qt.io/)
- [KVM/QEMU Documentation](https://www.linux-kvm.org/page/Documents)

---

*Last Updated: 2026-01-21*
*For QVirt-Manager version 1.0.0*
