# Core library
add_library(qvirt-core
    core/BaseObject.cpp
    core/BaseUIObject.cpp
    core/Engine.cpp
    core/Error.cpp
    core/Config.cpp
    core/SystemTray.cpp
    core/ProgressDialog.cpp
    core/GuestAgent.cpp
)

target_include_directories(qvirt-core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LIBVIRT_FOUND)
    target_include_directories(qvirt-core
        PUBLIC
            ${LIBVIRT_INCLUDE_DIRS}
    )
    target_compile_definitions(qvirt-core
        PUBLIC
            LIBVIRT_FOUND
    )
endif()

target_link_libraries(qvirt-core
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
)

if(LIBVIRT_FOUND)
    target_link_libraries(qvirt-core
        PRIVATE
            ${LIBVIRT_LIBRARIES}
    )
endif()

# Libvirt wrappers
if(LIBVIRT_FOUND)
    add_library(qvirt-libvirt
        libvirt/Connection.cpp
        libvirt/Domain.cpp
        libvirt/DomainSnapshot.cpp
        libvirt/DomainCheckpoint.cpp
        libvirt/Network.cpp
        libvirt/StoragePool.cpp
        libvirt/StorageVolume.cpp
        libvirt/NodeDevice.cpp
        libvirt/EnumMapper.cpp
        libvirt/Guest.cpp
    )
else()
    add_library(qvirt-libvirt STATIC
        libvirt/DomainCheckpoint.cpp
    )
endif()

target_include_directories(qvirt-libvirt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qvirt-libvirt
    PUBLIC
        qvirt-core
        qvirt-devices
        qvirt-domain
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
)

# Console library
add_library(qvirt-console
    console/Viewer.cpp
    console/VNCViewer.cpp
    console/SpiceViewer.cpp
    console/VNCWidget.cpp
    console/SpiceWidget.cpp
    console/SerialConsole.cpp
)

target_include_directories(qvirt-console
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qvirt-console
    PUBLIC
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Gui
)

if(GTKVNC_FOUND)
    target_include_directories(qvirt-console
        PRIVATE
            ${GTKVNC_INCLUDE_DIRS}
    )
    target_link_libraries(qvirt-console
        PRIVATE
            ${GTKVNC_LIBRARIES}
    )
    target_compile_definitions(qvirt-console
        PUBLIC
            GTKVNC_FOUND
    )
endif()

if(SPICEGTK_FOUND)
    target_include_directories(qvirt-console
        PRIVATE
            ${SPICEGTK_INCLUDE_DIRS}
    )
    target_link_libraries(qvirt-console
        PRIVATE
            ${SPICEGTK_LIBRARIES}
    )
    target_compile_definitions(qvirt-console
        PUBLIC
            SPICEGTK_FOUND
    )
endif()

if(QTERMWIDGET_FOUND)
    target_include_directories(qvirt-console
        PRIVATE
            ${QTERMWIDGET_INCLUDE_DIRS}
    )
    target_link_libraries(qvirt-console
        PRIVATE
            ${QTERMWIDGET_LIBRARIES}
    )
    target_compile_definitions(qvirt-console
        PUBLIC
            QTERMWIDGET_FOUND
    )
endif()

# Device library
add_library(qvirt-devices
    devices/Device.cpp
    devices/DiskDevice.cpp
    devices/NetworkDevice.cpp
    devices/ControllerDevice.cpp
    devices/InputDevice.cpp
    devices/GraphicsDevice.cpp
    devices/VideoDevice.cpp
    devices/SoundDevice.cpp
    devices/TPMDevice.cpp
    devices/HostDevice.cpp
    devices/FileSystemDevice.cpp
    devices/WatchdogDevice.cpp
    devices/RNGDevice.cpp
    devices/SmartcardDevice.cpp
    devices/MemballoonDevice.cpp
    devices/PanicDevice.cpp
    devices/VSockDevice.cpp
    devices/USBRedirDevice.cpp
    devices/SerialDevice.cpp
    devices/ParallelDevice.cpp
    devices/IOMMUDevice.cpp
    devices/ChannelDevice.cpp
)

target_include_directories(qvirt-devices
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qvirt-devices
    PUBLIC
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
)

# Domain library
add_library(qvirt-domain
    domain/CPUTune.cpp
    domain/MemTune.cpp
    domain/BlkIOTune.cpp
    domain/NUMA.cpp
    domain/MemoryBacking.cpp
    domain/Firmware.cpp
    domain/BootConfig.cpp
    domain/Clock.cpp
    domain/Features.cpp
    domain/SecLabel.cpp
    domain/LaunchSecurity.cpp
    domain/CloudInit.cpp
    domain/IOThread.cpp
)

target_include_directories(qvirt-domain
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(qvirt-domain
    PUBLIC
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Xml
)

# UI components
add_library(qvirt-ui
    ui/manager/ManagerWindow.cpp
    ui/models/ConnectionListModel.cpp
    ui/models/ConnectionTreeModel.cpp
    ui/models/VMListModel.cpp
    ui/models/VMProxyModel.cpp
    ui/dialogs/ConnectionDialog.cpp
    ui/dialogs/AddHardwareDialog.cpp
    ui/dialogs/CustomizeGuestDialog.cpp
    ui/dialogs/HostDialog.cpp
    ui/dialogs/DeleteDialog.cpp
    ui/dialogs/StoragePoolDialog.cpp
    ui/dialogs/NetworkDialog.cpp
    ui/dialogs/SnapshotDialog.cpp
    ui/dialogs/CloneDialog.cpp
    ui/dialogs/MigrateDialog.cpp
    ui/dialogs/PreferencesDialog.cpp
    ui/dialogs/XMLEditor.cpp
    ui/dialogs/ErrorDialog.cpp
    ui/dialogs/FirmwareDialog.cpp
    ui/dialogs/BootOrderDialog.cpp
    ui/dialogs/CPUTuneDialog.cpp
    ui/dialogs/MemTuneDialog.cpp
    ui/dialogs/BlkIOTuneDialog.cpp
    ui/widgets/GraphWidget.cpp
    ui/widgets/ContextMenu.cpp
    ui/widgets/VMFilterBar.cpp
    ui/widgets/ExportStatsDialog.cpp
    ui/widgets/ConsoleToolbar.cpp
    ui/device/GuestAgentDetails.cpp
    ui/vmwindow/VMWindow.cpp
    ui/vmwindow/OverviewPage.cpp
    ui/vmwindow/DetailsPage.cpp
    ui/vmwindow/ConsolePage.cpp
    ui/vmwindow/SnapshotsPage.cpp
    ui/wizards/CreateVMWizard.cpp
    ui/wizards/CreatePoolDialog.cpp
    ui/wizards/CreateNetworkWizard.cpp
    ui/wizards/CreateVolumeWizard.cpp
)

target_include_directories(qvirt-ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LIBVIRT_FOUND)
    target_include_directories(qvirt-ui
        PUBLIC
            ${LIBVIRT_INCLUDE_DIRS}
    )
    target_compile_definitions(qvirt-ui
        PUBLIC
            LIBVIRT_FOUND
    )
endif()

target_link_libraries(qvirt-ui
    PUBLIC
        qvirt-core
        qvirt-libvirt
        qvirt-console
        qvirt-devices
        qvirt-domain
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
        Qt${QT_VERSION_MAJOR}::Xml
)

# Main executable
add_executable(qvirt-manager
    main.cpp
    ../resources/qvirt.qrc
)

target_include_directories(qvirt-manager
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LIBVIRT_FOUND)
    target_include_directories(qvirt-manager
        PRIVATE
            ${LIBVIRT_INCLUDE_DIRS}
    )
    target_link_directories(qvirt-manager
        PRIVATE
            ${LIBVIRT_LIBRARY_DIRS}
    )
endif()

target_link_libraries(qvirt-manager
    PRIVATE
        qvirt-ui
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Network
)

# Install
install(TARGETS qvirt-manager DESTINATION ${CMAKE_INSTALL_BINDIR})
