# Core library
add_library(qvirt-core
    core/BaseObject.cpp
    core/BaseUIObject.cpp
    core/Engine.cpp
    core/Error.cpp
    core/Config.cpp
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
endif()

target_link_libraries(qvirt-core
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
)

if(LIBVIRT_FOUND)
    target_link_libraries(qvirt-core
        PUBLIC
            ${LIBVIRT_LIBRARIES}
    )
endif()

# Libvirt wrappers
add_library(qvirt-libvirt
    libvirt/Connection.cpp
    libvirt/Domain.cpp
    libvirt/DomainSnapshot.cpp
    libvirt/Network.cpp
    libvirt/StoragePool.cpp
    libvirt/StorageVolume.cpp
    libvirt/NodeDevice.cpp
    libvirt/EnumMapper.cpp
    libvirt/Guest.cpp
)

target_include_directories(qvirt-libvirt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(LIBVIRT_FOUND)
    target_include_directories(qvirt-libvirt
        PUBLIC
            ${LIBVIRT_INCLUDE_DIRS}
    )
endif()

target_link_libraries(qvirt-libvirt
    PUBLIC
        qvirt-core
        qvirt-devices
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Xml
)

if(LIBVIRT_FOUND)
    target_link_libraries(qvirt-libvirt
        PUBLIC
            ${LIBVIRT_LIBRARIES}
    )
endif()

# Console library
add_library(qvirt-console
    console/Viewer.cpp
    console/VNCViewer.cpp
    console/SpiceViewer.cpp
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
)

# UI components
add_library(qvirt-ui
    ui/manager/ManagerWindow.cpp
    ui/models/ConnectionListModel.cpp
    ui/models/VMListModel.cpp
    ui/dialogs/ConnectionDialog.cpp
    ui/dialogs/AddHardwareDialog.cpp
    ui/dialogs/HostDialog.cpp
    ui/dialogs/DeleteDialog.cpp
    ui/dialogs/StoragePoolDialog.cpp
    ui/dialogs/NetworkDialog.cpp
    ui/dialogs/SnapshotDialog.cpp
    ui/dialogs/CloneDialog.cpp
    ui/dialogs/MigrateDialog.cpp
    ui/dialogs/PreferencesDialog.cpp
    ui/dialogs/XMLEditor.cpp
    ui/widgets/GraphWidget.cpp
    ui/widgets/ContextMenu.cpp
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
endif()

target_link_libraries(qvirt-ui
    PUBLIC
        qvirt-core
        qvirt-libvirt
        qvirt-console
        qvirt-devices
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
endif()

if(LIBVIRT_FOUND)
    target_link_directories(qvirt-manager
        PRIVATE
            ${LIBVIRT_LIBRARY_DIRS}
    )
endif()

target_link_libraries(qvirt-manager
    PRIVATE
        qvirt-core
        qvirt-libvirt
        qvirt-devices
        qvirt-ui
        # Explicitly link Qt libraries to ensure proper symbol resolution on all platforms
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
        Qt${QT_VERSION_MAJOR}::Xml
        Qt${QT_VERSION_MAJOR}::Network
)

if(LIBVIRT_FOUND)
    target_link_libraries(qvirt-manager
        PRIVATE
            ${LIBVIRT_LIBRARIES}
    )
endif()

# Install
install(TARGETS qvirt-manager DESTINATION ${CMAKE_INSTALL_BINDIR})
