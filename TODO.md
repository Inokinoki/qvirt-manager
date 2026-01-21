# QVirt-Manager Implementation TODO

## Overview

Create **qvirt-manager**, a C++ Qt-based replacement for the Python GTK+ virt-manager application.

## Project Configuration

- **Language**: C++17
- **Qt Version**: Qt 5.15+ or Qt 6.2+ (support both)
- **Build System**: CMake 3.16+
- **Approach**: Incremental implementation in phases
- **Structure**: Standalone package under `qvirt-manager/`
- **Goal**: Full feature parity with virt-manager

---

## Phase 1: Foundation ✅ COMPLETE

**Status**: Completed
**Deliverable**: Basic project structure, libvirt connection, simple UI

### Completed Tasks

- [x] Set up CMake build system with Qt 5/6 detection
- [x] Create base classes (BaseObject, BaseUIObject)
- [x] Implement libvirt connection wrapper with RAII
- [x] Create simple main window with connection list
- [x] Implement configuration system (QSettings)
- [x] Basic error dialogs

### Files Created

- `CMakeLists.txt` - Root CMake configuration
- `src/CMakeLists.txt` - Source CMake configuration
- `src/main.cpp` - Application entry point
- `src/core/BaseObject.h/cpp` - Base class for all objects
- `src/core/BaseUIObject.h/cpp` - Base class for UI components
- `src/core/Engine.h/cpp` - Application singleton
- `src/core/Error.h/cpp` - Error dialog handling
- `src/core/Config.h/cpp` - Configuration manager (QSettings)
- `src/libvirt/Connection.h/cpp` - Libvirt connection wrapper (stub)
- `src/libvirt/Domain.h/cpp` - Domain wrapper (stub)
- `src/ui/manager/ManagerWindow.h/cpp` - Main window (stub)
- `cmake/Qt5Config.cmake` - Qt 5 configuration
- `cmake/Qt6Config.cmake` - Qt 6 configuration
- `cmake/LibvirtConfig.cmake` - Libvirt configuration

---

## Phase 2: Connection and Domain Management ✅ COMPLETE

**Status**: Completed
**Deliverable**: Full connection management, domain listing, basic VM operations

### Completed Tasks

- [x] Complete Connection implementation with polling
- [x] Implement Domain wrapper class with lifecycle operations
- [x] Implement Network, StoragePool, NodeDevice wrappers
- [x] Create VM list view with filtering
- [x] Add VM start/stop/reboot functionality
- [x] Implement VM statistics collection
- [x] Connection manager dialog
- [x] Qt Model/View architecture implementation
- [x] Real-time VM state updates via signals/slots

### Files Created

- `src/libvirt/Connection.h/cpp` - Enhanced with polling, domain listing
- `src/libvirt/Domain.h/cpp` - Complete VM lifecycle (start/stop/reboot/reset/destroy/save)
- `src/libvirt/Network.h/cpp` - Virtual network wrapper
- `src/libvirt/StoragePool.h/cpp` - Storage pool wrapper
- `src/libvirt/NodeDevice.h/cpp` - Node device wrapper
- `src/libvirt/EnumMapper.h/cpp` - Libvirt enum to string conversion
- `src/ui/models/ConnectionListModel.h/cpp` - Connection list model
- `src/ui/models/VMListModel.h/cpp` - VM list model with filtering
- `src/ui/dialogs/ConnectionDialog.h/cpp` - Add connection dialog
- `src/ui/manager/ManagerWindow.h/cpp` - Complete manager UI

### Features Implemented

- VM lifecycle: start, stop, reboot, reset, destroy, save
- Connection management: add/remove connections, autoconnect
- VM filtering: show/hide active and inactive VMs
- Real-time updates: CPU usage, memory usage, state changes
- Qt UI: Toolbar, menus, status bar, control buttons
- Configuration persistence: QSettings for URIs and preferences

---

## Phase 3: VM Details Window ✅ COMPLETE

**Status**: Completed
**Deliverable**: VM details window with hardware information

### Tasks

- [x] Create VMWindow class framework
- [x] Implement overview page (VM info, stats)
- [x] Implement hardware details page with device tree
- [ ] Add device editing (remove, modify) - Phase 6
- [x] Display CPU, memory, disk, network info
- [ ] Performance graphs for CPU/memory - Phase 9
- [x] Multi-window management (one window per VM)
- [x] Tab-based interface (Overview, Details, Console, Snapshots)

### Files Created

- `src/ui/vmwindow/VMWindow.h/cpp` - VM details window
- `src/ui/vmwindow/OverviewPage.h/cpp` - VM overview and stats
- `src/ui/vmwindow/DetailsPage.h/cpp` - Hardware details tab
- `src/devices/Device.h/cpp` - Base device class
- `src/devices/DiskDevice.h/cpp` - Disk device handling
- `src/devices/NetworkDevice.h/cpp` - Network device handling
- `src/devices/ControllerDevice.h/cpp` - Controller device handling
- `src/devices/InputDevice.h/cpp` - Input device handling

### Key Features Implemented

- Multi-window support (open multiple VM windows)
- Tab widget: Overview, Details, Console (placeholder), Snapshots (placeholder)
- Device tree with expandable categories
- Device add/remove/modify buttons (UI only, implementation in Phase 6)
- Live stats display (CPU %, memory usage)
- VM controls (Start, Stop, Reboot, Pause, Force Off)

---

## Phase 4: Console Viewers 📋 TODO

**Status**: Pending
**Deliverable**: VNC and SPICE console viewing

### Tasks

- [ ] Implement Viewer base class
- [ ] Create VNCViewer using libvncclient
- [ ] Create SpiceViewer using SpiceGtk
- [ ] Implement console page in VMWindow
- [ ] Add keyboard/mouse grab handling
- [ ] Fullscreen mode support
- [ ] Screenshot functionality
- [ ] USB device redirection
- [ ] SSH tunnel support for remote connections

### Files to Create

- `src/console/Viewer.h/cpp` - Base viewer interface
- `src/console/VNCViewer.h/cpp` - VNC implementation
- `src/console/SpiceViewer.h/cpp` - SPICE implementation
- `src/console/SSHTunnels.h/cpp` - SSH tunnel management
- `src/ui/vmwindow/ConsolePage.h/cpp` - Console tab UI
- `src/ui/widgets/VNCViewer.h/cpp` - VNC widget
- `src/ui/widgets/SpiceViewer.h/cpp` - SPICE widget

### Dependencies

- **VNC**: libvncclient (lightweight) or GtkVnc
- **SPICE**: Spice GTK

---

## Phase 5: VM Creation Wizard 🔄 IN PROGRESS

**Status**: In Progress (UI Complete, VM Creation Backend Pending)
**Deliverable**: Full-featured VM creation wizard

### Tasks

- [x] Create wizard framework
- [x] Step 1: Name and OS selection
- [x] Step 2: Installation media (ISO, URL, import, PXE, container)
- [x] Step 3: Memory and CPU configuration
- [x] Step 4: Storage configuration (create, browse, select)
- [x] Step 5: Network configuration
- [x] Step 6: Final summary and installation
- [ ] Integrate with OS database (libosinfo)
- [ ] Custom XML option for advanced users
- [ ] Implement actual VM creation backend

### Files Created

- `src/ui/wizards/CreateVMWizard.h/cpp` - Main wizard (1,279 lines)

### Key Features Implemented

- Multi-step wizard with navigation using QWizard
- OS type/version selection with Linux/Windows/UNIX options
- Installation source: ISO, URL, PXE, import, container boot
- Storage: Create new disk, use existing, no storage
- Network: Virtual network, bridge, macvtap, direct, isolated
- Custom CPU, memory settings with spin boxes
- Disk format (qcow2, raw, vmdk, vdi) and bus selection
- Summary page showing all configuration

### Remaining Work

- Connect wizard's Finish button to actual VM XML generation
- Define VM using virDomainDefineXML()
- Start VM installation
- OS auto-detection from ISO media
- Storage pool browser (Phase 7)
- Advanced options (firmware, chipset, architecture)

---

## Phase 6: Advanced Hardware Management 📋 TODO

**Status**: Pending
**Deliverable**: Add hardware dialog, all device types

### Tasks

- [ ] Create AddHardwareDialog framework
- [ ] Disk device: various formats (qcow2, raw, vmdk), buses (virtio, ide, scsi, sata)
- [ ] Network device: bridge, NAT, macvtap, direct
- [ ] Controllers: USB, SCSI, SATA, PCI, IDE
- [ ] Input devices: tablet, keyboard, mouse
- [ ] Graphics: VNC, SPICE, SDL
- [ ] Sound devices
- [ ] Video devices (qxl, virtio, cirrus)
- [ ] Host device passthrough (USB, PCI)
- [ ] TPM, vsock, watchdog, smartcard
- [ ] Filesystem passthrough (virtio-fs)
- [ ] XML editor for advanced users
- [ ] Device removal with confirmation

### Files to Create

- `src/ui/wizards/AddHardwareDialog.h/cpp` - Add hardware wizard
- `src/devices/DiskDevice.h/cpp` - Disk device
- `src/devices/NetworkDevice.h/cpp` - Network device
- `src/devices/ControllerDevice.h/cpp` - Controller device
- `src/devices/InputDevice.h/cpp` - Input device
- `src/devices/GraphicsDevice.h/cpp` - Graphics device
- `src/devices/SoundDevice.h/cpp` - Sound device
- `src/devices/VideoDevice.h/cpp` - Video device
- `src/devices/HostDevice.h/cpp` - Host device passthrough
- `src/devices/TPMDevice.h/cpp` - TPM device
- `src/ui/widgets/[various].h/cpp` - Device-specific widgets
- `src/utils/XMLHelper.h/cpp` - XML processing utilities
- `src/ui/dialogs/XMLEditor.h/cpp` - XML editor dialog

### Device Types

1. **Storage**: Disk, CDROM, Floppy, LUN
2. **Network**: Network interface, bridge
3. **Controllers**: USB, SCSI, SATA, IDE, PCI, virtio-serial
4. **Input**: Tablet, Keyboard, Mouse
5. **Graphics**: VNC, SPICE, SDL
6. **Sound**: ich6, ich9, ac97
7. **Video**: qxl, virtio, cirrus, vmvga
8. **Host Devices**: USB, PCI passthrough
9. **Special**: TPM, vsock, watchdog, smartcard, filesystem, RNG

---

## Phase 7: Storage and Network Management 📋 TODO

**Status**: Pending
**Deliverable**: Storage pool and network management

### Tasks

#### Storage Management
- [ ] Create storage pool browser
- [ ] Storage volume operations (create, delete, clone, upload, download)
- [ ] Storage pool wizard (create, define, start, stop, delete)
- [ ] Volume creation wizard with size, format, allocation
- [ ] Volume clone and wipe operations
- [ ] Support various pool types: dir, fs, logical, disk, iscsi, iscsi-direct, rbd, sheepdog, glusterfs, zfs

#### Network Management
- [ ] Create network browser
- [ ] Virtual network operations (start, stop, delete, undefine)
- [ ] Network wizard (create NAT, route, isolated, bridge networks)
- [ ] DHCP configuration
- [ ] Static IP mapping
- [ ] IPv6 support

#### Host Details
- [ ] Create host details window
- [ ] Show system information (CPU, memory, hypervisor version)
- [ ] List all node devices
- [ ] Performance overview

### Files to Create

- `src/ui/dialogs/HostDialog.h/cpp` - Host details window
- `src/ui/wizards/CreatePoolDialog.h/cpp` - Storage pool wizard
- `src/ui/wizards/CreateNetworkDialog.h/cpp` - Network wizard
- `src/ui/wizards/CreateVolumeDialog.h/cpp` - Volume creation wizard
- `src/ui/widgets/StoragePoolList.h/cpp` - Storage pool browser
- `src/ui/widgets/NetworkList.h/cpp` - Network browser
- `src/ui/widgets/VolumeBrowser.h/cpp` - Volume browser

---

## Phase 8: Snapshots and Advanced Features 📋 TODO

**Status**: Pending
**Deliverable**: Snapshot management, cloning, migration

### Tasks

#### Snapshot Management
- [ ] Create snapshot manager
- [ ] Take snapshots (with memory state)
- [ ] Revert to snapshot
- [ ] Delete snapshots
- [ ] View snapshot details (XML, timestamp, description)
- [ ] Snapshot tree visualization

#### VM Cloning
- [ ] Clone VM wizard
- [ ] Clone storage volumes
- [ ] Generate new MAC addresses
- [ ] Clone all devices or selective

#### VM Migration
- [ ] Migrate VM wizard
- [ ] Live migration support
- [ ] Peer-to-peer migration
- [ ] Tunnel migration over SSH
- [ ] Migration options: offline, live, persistent

#### Deletion
- [ ] Delete VM confirmation dialog
- [ ] Delete storage option
- [ ] Force delete option

#### Preferences
- [ ] Preferences dialog
- [ ] Default connection URIs
- [ ] Default storage location
- [ ] Console scaling preferences
- [ ] Keyboard grab behavior
- [ ] Update interval settings

### Files to Create

- `src/ui/vmwindow/SnapshotsPage.h/cpp` - Snapshot management tab
- `src/objects/DomainSnapshot.h/cpp` - VM snapshot wrapper
- `src/ui/dialogs/CloneDialog.h/cpp` - Clone VM wizard
- `src/ui/dialogs/MigrateDialog.h/cpp` - Migrate VM wizard
- `src/ui/dialogs/DeleteDialog.h/cpp` - Delete confirmation
- `src/ui/dialogs/PreferencesDialog.h/cpp` - Preferences dialog

---

## Phase 9: Polish and Optimization 📋 TODO

**Status**: Pending
**Deliverable**: Refined UI, performance optimization

### Tasks

#### Visual Improvements
- [ ] Add application icon
- [ ] Add status icons (running, paused, stopped)
- [ ] Add device type icons
- [ ] Performance graphs (CPU, memory, disk, network)
- [ ] Tooltip improvements
- [ ] Context-sensitive help text

#### Performance
- [ ] Optimize polling intervals
- [ ] Reduce unnecessary UI updates
- [ ] Lazy-load device lists
- [ ] Cache frequently accessed data
- [ ] Background thread for statistics

#### User Experience
- [ ] Keyboard shortcuts for all common operations
- [ ] Right-click context menus
- [ ] Drag-and-drop support
- [ ] Recent connections list
- [ ] Quick search/filter for VMs

#### Internationalization
- [ ] Extract translatable strings
- [ ] Create translation files (.ts)
- [ ] Support multiple languages
- [ ] Translation packaging

### Files to Create

- `src/ui/widgets/GraphWidget.h/cpp` - Performance graph widget
- `src/ui/delegates/SparklineDelegate.h/cpp` - Sparkline chart delegate
- `resources/icons/` - All application icons
- `resources/styles/default.qss` - Default stylesheet
- `resources/i18n/` - Translation files
- `resources/i18n/qvirt-manager_en.ts` - English (template)
- `resources/i18n/qvirt-manager_de.ts` - German
- `resources/i18n/qvirt-manager_fr.ts` - French
- `resources/i18n/qvirt-manager_es.ts` - Spanish

---

## Phase 10: Testing and Documentation 📋 TODO

**Status**: Pending
**Deliverable**: Stability assurance, complete documentation

### Tasks

#### Unit Tests
- [ ] Unit tests for core classes
- [ ] Unit tests for libvirt wrappers
- [ ] Unit tests for UI models
- [ ] Mock libvirt for testing

#### Integration Tests
- [ ] Connection management tests
- [ ] VM lifecycle tests
- [ ] Device management tests
- [ ] Wizard completion tests

#### Performance Testing
- [ ] Memory leak detection (Valgrind)
- [ ] Large VM list performance
- [ ] Long-running stability tests
- [ ] Resource usage profiling

#### Documentation
- [ ] User manual
- [ ] Installation guide
- [ ] Configuration reference
- [ ] Developer documentation
- [ ] API documentation (Doxygen)

#### Packaging
- [ ] RPM spec file
- [ ] Debian packaging
- [ ] Flatpak manifest
- [ ] AppImage packaging
- [ ] Homebrew formula (macOS)
- [ ] Windows installer

### Files to Create

- `tests/unit/test_baseobject.cpp` - BaseObject tests
- `tests/unit/test_connection.cpp` - Connection tests
- `tests/unit/test_domain.cpp` - Domain tests
- `tests/unit/test_models.cpp` - Model tests
- `tests/integration/test_vm_lifecycle.cpp` - VM lifecycle tests
- `tests/integration/test_connection_manager.cpp` - Connection tests
- `tests/mocks/libvirt_mock.h` - Libvirt mocking
- `doc/user-manual.md` - User documentation
- `doc/installation.md` - Installation guide
- `doc/developer-guide.md` - Developer guide
- `doc/api-reference.md` - API documentation
- `packaging/qvirt-manager.spec` - RPM spec
- `packaging/debian/` - Debian packaging
- `packaging/flatpak/` - Flatpak manifest
- `packaging/appimage/` - AppImage build
- `packaging/homebrew/` - Homebrew formula

---

## Success Criteria

### Phase 1-2 Complete ✅
- [x] Can compile and run
- [x] Opens libvirt connection
- [x] Lists VMs
- [x] Start/stop VMs work
- [x] Clean Qt UI with toolbar and menus

### Phase 3-4 Complete 📋
- [x] Manager UI with VM list
- [x] VM details window with tabs
- [ ] Console viewers work (VNC/SPICE)
- [x] Can view and edit hardware details
- [ ] Performance graphs

### Phase 5 Complete 📋
- [ ] Can create new VMs via wizard
- [ ] All installation methods work
- [ ] OS auto-detection
- [ ] Storage and network configuration

### Final Success 📋
- [ ] 90%+ feature parity with virt-manager
- [ ] All device types supported
- [ ] Storage/network management complete
- [ ] Snapshot support
- [ ] Cloning and migration
- [ ] Passes all tests
- [ ] Performance >= Python version
- [ ] Complete documentation
- [ ] Multi-platform support (Linux, macOS, Windows)

---

## Implementation Notes

1. **Signal/Slot Pattern**: Use Qt's signal/slot system instead of GTK callbacks
2. **UI Files**: Use Qt Designer .ui files (load via QUiLoader)
3. **Threading**: Use QThread, QThreadPool with main-thread UI updates via QMetaObject::invokeMethod
4. **XML Processing**: Use Qt XML (QDomDocument) or port virtinst XML logic
5. **Configuration**: Use QSettings instead of GSettings
6. **Console Viewers**: Embed SpiceGtk/GtkVnc widgets or use pure Qt alternatives
7. **Memory Management**: RAII, Qt parent-child relationships, smart pointers

---

## Dependencies

**Required**:
- Qt 5.15+ or Qt 6.2+ (Widgets, Core, Network, UiTools, XML)
- libvirt >= 4.5.0 (C API)
- CMake >= 3.16
- C++17 compiler (GCC 8+, Clang 10+, MSVC 2019+)

**Optional**:
- Spice GTK >= 0.35 (SPICE console)
- GtkVnc >= 1.0 (VNC console)
- libosinfo (OS detection)
- libguestfs (VM inspection)
- libvncclient (lightweight VNC)

---

## Estimated Effort

- **1 developer**: 40-50 weeks (10-12 months)
- **2 developers**: 20-25 weeks (5-6 months)
- **3 developers**: 15-18 weeks (4-5 months)

Estimated LOC: ~61,500 lines C++ (from 41,000 lines Python, 1.5x multiplier)

---

## Critical Reference Files from virt-manager

When implementing, reference these files for architecture and behavior:

1. **virtManager/baseclass.py** - Base architecture pattern
2. **virtManager/connection.py** - Connection management, polling logic
3. **virtManager/manager.py** - Main UI architecture, VM list model
4. **virtManager/vmwindow.py** - VM details window structure
5. **virtManager/createvm.py** - Wizard architecture
6. **virtManager/details/viewers.py** - Console viewer abstraction
7. **virtinst/guest.py** - Guest configuration model
8. **ui/manager.ui** - Main window layout reference
9. **ui/createvm.ui** - Complex wizard UI reference
