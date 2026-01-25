# QVirt-Manager User Manual

## Table of Contents

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Getting Started](#getting-started)
4. [Managing Connections](#managing-connections)
5. [Virtual Machine Management](#virtual-machine-management)
6. [Hardware Configuration](#hardware-configuration)
7. [Storage Management](#storage-management)
8. [Network Management](#network-management)
9. [Snapshots](#snapshots)
10. [Preferences](#preferences)
11. [Troubleshooting](#troubleshooting)

---

## Introduction

QVirt-Manager is a Qt-based virtual machine manager that provides a graphical interface for managing libvirt VMs. It is a modern C++ replacement for virt-manager.

### Features

- **VM Lifecycle Management**: Start, stop, reboot, pause, and save VMs
- **VM Creation**: Easy-to-use wizard for creating new VMs
- **Hardware Management**: Add, edit, and remove virtual hardware
- **Storage Management**: Manage storage pools and volumes
- **Network Management**: Configure virtual networks
- **Snapshots**: Create and manage VM snapshots
- **Console Viewing**: View and control VM consoles (VNC/SPICE)
- **Multi-Connection**: Connect to multiple hypervisors

---

## Installation

### From Source

```bash
# Install dependencies
sudo apt-get install cmake qtbase5-dev qttools5-dev libvirt-dev pkg-config

# Build
mkdir build && cd build
cmake ..
make

# Install
sudo make install
```

### From Packages

**Fedora/RHEL/CentOS:**
```bash
sudo dnf install qvirt-manager
```

**Debian/Ubuntu:**
```bash
sudo apt install qvirt-manager
```

**Flatpak:**
```bash
flatpak install flathub com.virtmanager.QVirt
```

---

## Getting Started

### First Launch

When you first launch QVirt-Manager, you'll see the main window with two panels:

1. **Left Panel**: Connection list
2. **Right Panel**: Virtual machine list

### Adding a Connection

1. Click **File → Add Connection** or click the **Add Connection** button
2. Select the hypervisor type (QEMU/KVM is default)
3. Choose connection method:
   - **Local System**: `qemu:///system`
   - **Remote SSH**: `qemu+ssh://user@host/system`
   - **TCP**: `qemu+tcp://host:16509/system`
4. Check **Auto-connect** to automatically connect on startup
5. Click **Connect**

---

## Managing Connections

### Connection Operations

- **Add Connection**: Add a new hypervisor connection
- **Remove Connection**: Remove a connection from the list
- **Auto-connect**: Automatically connect when application starts

### Connection States

Connections can be in the following states:
- **Active**: Connected and responsive
- **Inactive**: Not connected
- **Error**: Connection failed

---

## Virtual Machine Management

### VM List

The VM list displays all VMs with the following information:
- **Name**: VM name
- **State**: Current state (Running, Paused, Stopped)
- **CPU**: CPU usage percentage
- **Memory**: Memory usage

### VM Operations

#### Starting a VM

1. Select the VM in the list
2. Click **Start** or press **Ctrl+S**

#### Stopping a VM

1. Select the VM
2. Click **Stop** or press **Ctrl+T**

Gracefully shuts down the VM by sending an ACPI signal.

#### Rebooting a VM

1. Select the VM
2. Click **Reboot** or press **Ctrl+R**

#### Pausing a VM

1. Select a running VM
2. Click **Pause** or press **Ctrl+P**

Pauses the VM execution, preserving its state in memory.

#### Deleting a VM

1. Select the VM
2. Click **Delete** or press **Delete**
3. Choose whether to delete storage files
4. Confirm deletion

**Warning**: This operation cannot be undone!

---

## Hardware Configuration

### Viewing Hardware

1. Double-click a VM or select **View Details**
2. Go to the **Details** tab
3. Expand device categories to view hardware

### Adding Hardware

1. Open VM details
2. Click **Add Hardware**
3. Select hardware type:
   - **Storage**: Disk, CDROM, Floppy
   - **Network**: Network interface
   - **Controller**: USB, SCSI, SATA, PCI
   - **Input**: Tablet, Keyboard, Mouse
   - **Graphics**: VNC, SPICE, SDL
   - **Sound**: Audio devices
   - **Video**: Video card
4. Configure device properties
5. Click **Finish**

### Removing Hardware

1. Select the device in the device tree
2. Click **Remove**
3. Confirm removal

**Note**: Some core devices (CPU, Memory) cannot be removed.

---

## Storage Management

### Accessing Storage Pools

1. Click **View → Storage Pools**
2. Select a connection to manage

### Storage Pool Types

- **Directory**: Directory-based storage
- **Filesystem**: Mounted filesystem
- **Logical**: LVM volume group
- **Disk**: Physical disk
- **iSCSI**: iSCSI target
- **RBD**: Ceph/RBD

### Pool Operations

- **Start**: Activate a pool
- **Stop**: Deactivate a pool
- **Delete**: Remove a pool definition
- **Refresh**: Update volume list

### Volume Operations

- **Create Volume**: Create new disk image
- **Delete Volume**: Remove a volume
- **Clone Volume**: Copy a volume
- **Wipe Volume**: Securely erase volume data

---

## Network Management

### Accessing Networks

1. Click **View → Virtual Networks**
2. Select a connection to manage

### Network Types

- **NAT**: Network address translation (default)
- **Routed**: Direct routing without NAT
- **Isolated**: No external connectivity
- **Bridge**: Bridge to host network

### Network Operations

- **Start**: Activate a network
- **Stop**: Deactivate a network
- **Delete**: Remove network definition
- **Create**: Create new network

---

## Snapshots

### Creating Snapshots

1. Select a VM
2. Click **VM → Snapshots** or right-click → Take Snapshot
3. Enter snapshot name and description
4. Choose whether to include memory state
5. Click **Create**

**Memory State**: Captures the exact running state, including RAM contents.
Requires more disk space but allows resuming exactly where you left off.

### Reverting to a Snapshot

1. Open the Snapshots dialog
2. Select the snapshot
3. Click **Revert**
4. Confirm

**Warning**: Reverting will discard all changes since the snapshot.

### Deleting Snapshots

1. Select the snapshot
2. Click **Delete**
3. Confirm

---

## Preferences

Access preferences via **Edit → Preferences** or **Ctrl+,**

### General Settings

- **Confirm force power off**: Ask before forcing VM off
- **Confirm delete**: Ask before deleting VMs
- **Auto-connect**: Connect to saved connections on startup
- **Default storage path**: Default location for VM disk images

### Polling Intervals

- **VM update interval**: How often to refresh VM list (default: 2 seconds)
- **CPU stats interval**: CPU statistics polling (default: 1 second)
- **Disk stats interval**: Disk I/O statistics (default: 5 seconds)
- **Network stats interval**: Network statistics (default: 3 seconds)

Lower intervals provide more real-time updates but consume more resources.

### Console Settings

- **Scale display**: Scale console to fit window
- **Resize guest**: Allow console to resize guest resolution
- **Key grab**: Key combination to release keyboard/mouse
- **USB redirection**: Automatically redirect USB devices

---

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| **Ctrl+N** | New VM |
| **Delete** | Delete selected VM |
| **Ctrl+S** | Start VM |
| **Ctrl+T** | Stop VM |
| **Ctrl+R** | Reboot VM |
| **Ctrl+P** | Pause/Resume VM |
| **Ctrl+O** | Open Console |
| **F5** | Refresh |
| **Ctrl+,** | Preferences |
| **Ctrl+Q** | Quit |

---

## Troubleshooting

### Connection Issues

**Problem**: Cannot connect to hypervisor

**Solutions**:
- Ensure libvirtd is running: `sudo systemctl start libvirtd`
- Check user permissions: Add user to libvirt group
- Verify connection URI format
- Check firewall settings

### VM Won't Start

**Problem**: VM fails to start

**Solutions**:
- Check VM XML for errors
- Verify disk images exist and are accessible
- Ensure sufficient system resources
- Check libvirt logs: `journalctl -xeu libvirtd`

### Console Not Working

**Problem**: Cannot see VM console

**Solutions**:
- Ensure display is configured (VNC or SPICE)
- Check firewall ports (5900+ for VNC)
- Install spice-gtk or gtk-vnc packages
- Verify VM is running

### Performance Issues

**Problem**: Application is slow

**Solutions**:
- Increase polling intervals in preferences
- Reduce number of simultaneous connections
- Close unused VM detail windows
- Check system resources

---

## Getting Help

- **Documentation**: https://github.com/qvirt-manager/qvirt-manager/wiki
- **Issues**: https://github.com/qvirt-manager/qvirt-manager/issues
- **Email**: qvirt-manager@googlegroups.com
- **IRC**: #qvirt-manager on Libera.Chat

---

*QVirt-Manager Version 1.0.0*
*Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>*
