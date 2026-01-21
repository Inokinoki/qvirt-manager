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
        ${LIBVIRT_INCLUDE_DIRS}
)

target_link_libraries(qvirt-core
    PUBLIC
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
        ${LIBVIRT_LIBRARIES}
)

# Libvirt wrappers
add_library(qvirt-libvirt
    libvirt/Connection.cpp
    libvirt/Domain.cpp
    libvirt/Network.cpp
    libvirt/StoragePool.cpp
    libvirt/NodeDevice.cpp
    libvirt/EnumMapper.cpp
)

target_include_directories(qvirt-libvirt
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBVIRT_INCLUDE_DIRS}
)

target_link_libraries(qvirt-libvirt
    PUBLIC
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        ${LIBVIRT_LIBRARIES}
)

# UI components
add_library(qvirt-ui
    ui/manager/ManagerWindow.cpp
    ui/models/ConnectionListModel.cpp
    ui/models/VMListModel.cpp
    ui/dialogs/ConnectionDialog.cpp
)

target_include_directories(qvirt-ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBVIRT_INCLUDE_DIRS}
)

target_link_libraries(qvirt-ui
    PUBLIC
        qvirt-core
        qvirt-libvirt
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::UiTools
        Qt${QT_VERSION_MAJOR}::Xml
)

# Main executable
add_executable(qvirt-manager
    main.cpp
)

target_include_directories(qvirt-manager
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBVIRT_INCLUDE_DIRS}
)

target_link_directories(qvirt-manager
    PRIVATE
        ${LIBVIRT_LIBRARY_DIRS}
)

target_link_libraries(qvirt-manager
    PRIVATE
        qvirt-core
        qvirt-libvirt
        qvirt-ui
        ${LIBVIRT_LIBRARIES}
)

# Install
install(TARGETS qvirt-manager DESTINATION ${CMAKE_INSTALL_BINDIR})
