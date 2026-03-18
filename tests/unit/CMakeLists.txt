# Unit tests
# Note: Qt${QT_VERSION_MAJOR}Test is already found in parent tests/CMakeLists.txt

# Core library tests
# Note: qvirt-core depends on Engine which calls Connection::tick(), so we need qvirt-libvirt
add_executable(test_core test_core.cpp)
target_link_libraries(test_core
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_core PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_core COMMAND test_core)

# Config tests
add_executable(test_config test_config.cpp)
target_link_libraries(test_config
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_config PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_config COMMAND test_config)

# Device tests
add_executable(test_devices test_devices.cpp)
target_link_libraries(test_devices
    qvirt-libvirt
    qvirt-devices
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_devices PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_devices COMMAND test_devices)

# EnumMapper tests
add_executable(test_enummapper test_enummapper.cpp)
target_link_libraries(test_enummapper
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_enummapper PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_enummapper COMMAND test_enummapper)

# GraphWidget tests
add_executable(test_graphwidget test_graphwidget.cpp)
target_link_libraries(test_graphwidget
    qvirt-ui
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_graphwidget PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_graphwidget COMMAND test_graphwidget)

# NodeDevice tests
add_executable(test_nodedevice test_nodedevice.cpp)
target_link_libraries(test_nodedevice
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_nodedevice PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_nodedevice COMMAND test_nodedevice)

# Network tests
add_executable(test_network test_network.cpp)
target_link_libraries(test_network
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_network PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_network COMMAND test_network)

# StoragePool tests
add_executable(test_storagepool test_storagepool.cpp)
target_link_libraries(test_storagepool
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
)
target_link_directories(test_storagepool PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_storagepool COMMAND test_storagepool)
