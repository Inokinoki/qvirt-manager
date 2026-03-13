# Feature Parity Enhancement Plan

**Target:** Match virt-manager functionality  
**Branch:** `feat/feature-parity-enhancements`  
**Priority Areas:** Console Viewer, Config UIs, OS Database  
**Estimated Duration:** 6-9 weeks

---

## Phase 1: Console Viewer (Weeks 1-3)

### 1.1 VNC Viewer Integration
**Files to Create/Modify:**
- `src/ui/vmwindow/ConsolePage.cpp` - Replace placeholder
- `src/ui/vmwindow/ConsolePage.h`
- `src/console/VNCViewer.cpp` (NEW)
- `src/console/VNCViewer.h` (NEW)

**Dependencies:**
- gtk-vnc (libvncclient)
- Qt compatibility layer

**Implementation Steps:**
1. Add VNC library detection to CMakeLists.txt
2. Create VNCViewer wrapper class
3. Integrate VNC widget into ConsolePage
4. Implement connection/disconnection
5. Add scaling and fullscreen support
6. Implement send-key functionality

**Acceptance Criteria:**
- ✅ Connect to VNC console
- ✅ Display VM screen
- ✅ Keyboard/mouse input
- ✅ Scaling options
- ✅ Fullscreen mode

### 1.2 SPICE Viewer Integration
**Files to Create/Modify:**
- `src/console/SpiceViewer.cpp` (NEW)
- `src/console/SpiceViewer.h` (NEW)
- `src/ui/vmwindow/ConsolePage.cpp`

**Dependencies:**
- spice-gtk (Spice GTK library)

**Implementation Steps:**
1. Add Spice library detection to CMakeLists.txt
2. Create SpiceViewer wrapper class
3. Integrate SPICE widget into ConsolePage
4. Implement USB redirection
5. Add file transfer support
6. Implement smartcard support

**Acceptance Criteria:**
- ✅ Connect to SPICE console
- ✅ USB device redirection
- ✅ File transfer
- ✅ Smartcard passthrough

### 1.3 Serial Console
**Files to Create/Modify:**
- `src/ui/vmwindow/SerialConsolePage.cpp` (NEW)
- `src/ui/vmwindow/SerialConsolePage.h` (NEW)
- `src/console/SerialConsole.cpp` (NEW)
- `src/console/SerialConsole.h` (NEW)

**Implementation Steps:**
1. Create serial console widget
2. Implement PTY/socket connection
3. Add text terminal emulation
4. Support multiple serial ports

**Acceptance Criteria:**
- ✅ Connect to serial console
- ✅ Bidirectional text I/O
- ✅ Support multiple consoles

---

## Phase 2: OS Database Integration (Weeks 4-5)

### 2.1 libosinfo Integration
**Files to Create/Modify:**
- `src/core/OSDatabase.cpp` (NEW)
- `src/core/OSDatabase.h` (NEW)
- `src/ui/wizards/CreateVMWizard.cpp`
- `src/ui/wizards/CreateVMWizard.h`

**Dependencies:**
- libosinfo

**Implementation Steps:**
1. Add libosinfo detection to CMakeLists.txt
2. Create OSDatabase wrapper class
3. Implement OS detection from ISO/URL
4. Add OS filtering and search
5. Map OS to default configurations

**Acceptance Criteria:**
- ✅ Detect OS from install media
- ✅ Search/filter OS list
- ✅ Auto-configure VM for detected OS
- ✅ Show OS EOL warnings

### 2.2 OS Selection UI
**Files to Create/Modify:**
- `src/ui/dialogs/OSListDialog.cpp` (NEW)
- `src/ui/dialogs/OSListDialog.h` (NEW)

**Implementation Steps:**
1. Create searchable OS list widget
2. Implement OS filtering
3. Add EOL warning display
4. Integration with CreateVMWizard

**Acceptance Criteria:**
- ✅ Searchable OS list
- ✅ Filter by EOL status
- ✅ Display OS info

---

## Phase 3: Advanced Configuration UIs (Weeks 6-9)

### 3.1 Advanced Storage Configuration
**Files to Create/Modify:**
- `src/ui/dialogs/StorageConfigDialog.cpp` (NEW)
- `src/ui/dialogs/StorageConfigDialog.h` (NEW)
- `src/ui/dialogs/AddHardwareDialog.cpp`
- `src/devices/DiskDevice.cpp`
- `src/devices/DiskDevice.h`

**Implementation Steps:**
1. Create advanced storage config dialog
2. Add cache mode configuration
3. Add I/O mode settings
4. Implement discard/SSD support
5. Add advanced format options
6. Support disk bus selection

**Acceptance Criteria:**
- ✅ Configure disk cache modes
- ✅ Set I/O modes (native, threads)
- ✅ Enable/disable discard
- ✅ Select disk bus type
- ✅ Configure serial numbers

### 3.2 Advanced Graphics Configuration
**Files to Create/Modify:**
- `src/ui/dialogs/GraphicsConfigDialog.cpp` (NEW)
- `src/ui/dialogs/GraphicsConfigDialog.h` (NEW)
- `src/devices/GraphicsDevice.cpp`
- `src/devices/GraphicsDevice.h`

**Implementation Steps:**
1. Create graphics config dialog
2. Add multi-monitor support
3. Configure clipboard sharing
4. Add file transfer settings
5. Implement audio redirection
6. Support OpenGL acceleration

**Acceptance Criteria:**
- ✅ Multi-monitor configuration
- ✅ Clipboard sharing toggle
- ✅ File transfer settings
- ✅ Audio redirection
- ✅ 3D acceleration options

### 3.3 Advanced Network Configuration
**Files to Create/Modify:**
- `src/ui/dialogs/NetworkConfigDialog.cpp` (NEW)
- `src/ui/dialogs/NetworkConfigDialog.h` (NEW)
- `src/devices/NetworkDevice.cpp`
- `src/devices/NetworkDevice.h`
- `src/ui/dialogs/NetworkDialog.cpp`
- `src/ui/dialogs/NetworkDialog.h`

**Implementation Steps:**
1. Enhance network device dialog
2. Add MAC/VLAN settings
3. Implement QoS configuration
4. Add port forwarding UI
5. Support SR-IOV
6. Add network model selection

**Acceptance Criteria:**
- ✅ Configure MAC address
- ✅ Set VLAN tags
- ✅ Configure bandwidth limits
- ✅ Add port forwarding rules
- ✅ SR-IOV support

### 3.4 Storage Browser Dialog
**Files to Create/Modify:**
- `src/ui/dialogs/StorageBrowserDialog.cpp` (NEW)
- `src/ui/dialogs/StorageBrowserDialog.h` (NEW)

**Implementation Steps:**
1. Create tree view for pools/volumes
2. Implement volume filtering
3. Add volume operations (download, upload, clone, wipe)
4. Support format conversion
5. Integration with other dialogs

**Acceptance Criteria:**
- ✅ Browse all storage pools
- ✅ Filter volumes by format/type
- ✅ Download volumes
- ✅ Upload files as volumes
- ✅ Clone volumes
- ✅ Wipe volumes securely

### 3.5 Channel & Console Devices
**Files to Create/Modify:**
- `src/devices/ChannelDevice.cpp` (NEW)
- `src/devices/ChannelDevice.h` (NEW)
- `src/devices/ConsoleDevice.cpp` (NEW)
- `src/devices/ConsoleDevice.h` (NEW)
- `src/ui/dialogs/AddHardwareDialog.cpp`

**Implementation Steps:**
1. Create ChannelDevice class
2. Create ConsoleDevice class
3. Add to AddHardwareDialog
4. Support virtio-serial channels
5. Support spicevmc channels

**Acceptance Criteria:**
- ✅ Add channel devices
- ✅ Configure channel paths
- ✅ Support QEMU guest agent channel

---

## Build System Updates

### CMakeLists.txt Modifications

**Root CMakeLists.txt:**
```cmake
# Add optional dependencies
find_package(PkgConfig REQUIRED)

# VNC support (optional)
pkg_check_modules(GTKVNC gtk-vnc-1.0)
if(GTKVNC_FOUND)
    set(HAVE_GTK_VNC TRUE)
    add_definitions(-DHAVE_GTK_VNC)
endif()

# SPICE support (optional)
pkg_check_modules(SPICE spice-client-gtk-3.0)
if(SPICE_FOUND)
    set(HAVE_SPICE TRUE)
    add_definitions(-DHAVE_SPICE)
endif()

# libosinfo support (optional)
pkg_check_modules(LIBOSINFO libosinfo-1.0)
if(LIBOSINFO_FOUND)
    set(HAVE_LIBOSINFO TRUE)
    add_definitions(-DHAVE_LIBOSINFO)
endif()
```

---

## Testing Strategy

### Unit Tests
- Test OSDatabase queries
- Test device XML generation
- Test configuration dialogs

### Integration Tests
- Test VM creation with OS detection
- Test console connections (mocked)
- Test device hotplug

### Manual Testing
- Real VNC/SPICE console connections
- Real VM creation with various OSes
- Device configuration workflows

---

## Documentation Updates

### User Documentation
- Console viewer usage guide
- OS selection best practices
- Advanced configuration options

### Developer Documentation
- Console viewer architecture
- OS database API
- Device configuration patterns

---

## Risk Mitigation

### Technical Risks
1. **VNC/SPICE library compatibility**
   - Mitigation: Test with multiple library versions
   - Fallback: Keep placeholder as fallback option

2. **libosinfo availability**
   - Mitigation: Make optional, use built-in fallback
   - Fallback: Static OS list

3. **Qt/library version conflicts**
   - Mitigation: Test with Qt 5 and Qt 6
   - Fallback: Version-specific branches if needed

### Schedule Risks
1. **Console viewer complexity**
   - Mitigation: Start with VNC only, add SPICE later
   - Buffer: 1 week buffer in Phase 1

2. **Third-party API changes**
   - Mitigation: Abstract library interfaces
   - Buffer: Regular integration testing

---

## Success Metrics

### Functional Completeness
- [ ] VNC console fully functional
- [ ] SPICE console fully functional
- [ ] Serial console working
- [ ] OS detection from ISO/URL
- [ ] All advanced config dialogs implemented
- [ ] Storage browser operational

### Code Quality
- [ ] No TODO/FIXME comments in new code
- [ ] Unit tests for new features
- [ ] Documentation complete
- [ ] Build passes on Qt 5 and Qt 6

### Feature Parity
- [ ] Match virt-manager console features
- [ ] Match virt-manager OS selection
- [ ] Match virt-manager device configuration
- [ ] Exceed virt-manager in UX (Qt advantages)

---

## Appendix: File Manifest

### New Files (32 files)
```
src/console/VNCViewer.h
src/console/VNCViewer.cpp
src/console/SpiceViewer.h
src/console/SpiceViewer.cpp
src/console/SerialConsole.h
src/console/SerialConsole.cpp

src/core/OSDatabase.h
src/core/OSDatabase.h

src/devices/ChannelDevice.h
src/devices/ChannelDevice.cpp
src/devices/ConsoleDevice.h
src/devices/ConsoleDevice.cpp

src/ui/dialogs/OSListDialog.h
src/ui/dialogs/OSListDialog.cpp
src/ui/dialogs/StorageConfigDialog.h
src/ui/dialogs/StorageConfigDialog.cpp
src/ui/dialogs/GraphicsConfigDialog.h
src/ui/dialogs/GraphicsConfigDialog.cpp
src/ui/dialogs/NetworkConfigDialog.h
src/ui/dialogs/NetworkConfigDialog.cpp
src/ui/dialogs/StorageBrowserDialog.h
src/ui/dialogs/StorageBrowserDialog.cpp

src/ui/vmwindow/SerialConsolePage.h
src/ui/vmwindow/SerialConsolePage.cpp
```

### Modified Files (15 files)
```
src/ui/vmwindow/ConsolePage.h
src/ui/vmwindow/ConsolePage.cpp
src/ui/wizards/CreateVMWizard.h
src/ui/wizards/CreateVMWizard.cpp
src/ui/dialogs/AddHardwareDialog.h
src/ui/dialogs/AddHardwareDialog.cpp
src/ui/dialogs/NetworkDialog.h
src/ui/dialogs/NetworkDialog.cpp
src/devices/DiskDevice.h
src/devices/DiskDevice.cpp
src/devices/NetworkDevice.h
src/devices/NetworkDevice.cpp
src/devices/GraphicsDevice.h
src/devices/GraphicsDevice.cpp
CMakeLists.txt
src/CMakeLists.txt
```

---

**Created:** March 12, 2026  
**Status:** Ready for Implementation  
**Next Step:** Create branch and begin Phase 1
