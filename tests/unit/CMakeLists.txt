# Unit tests
find_package(Qt${QT_VERSION_MAJOR}Test ${QT_MIN_VERSION} QUIET)

if(Qt${QT_VERSION_MAJOR}Test_FOUND)
    # Core library tests
    add_executable(test_core test_core.cpp)
    target_link_libraries(test_core
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_core COMMAND test_core)

    # Config tests
    add_executable(test_config test_config.cpp)
    target_link_libraries(test_config
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_config COMMAND test_config)

    # Device tests
    add_executable(test_devices test_devices.cpp)
    target_link_libraries(test_devices
        qvirt-devices
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_devices COMMAND test_devices)

    # EnumMapper tests
    add_executable(test_enummapper test_enummapper.cpp)
    target_link_libraries(test_enummapper
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_enummapper COMMAND test_enummapper)

    # GraphWidget tests
    add_executable(test_graphwidget test_graphwidget.cpp)
    target_link_libraries(test_graphwidget
        qvirt-ui
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_graphwidget COMMAND test_graphwidget)

    # Migration dialog tests
    add_executable(test_migrations test_migrations.cpp)
    target_link_libraries(test_migrations
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_migrations COMMAND test_migrations)

    # Console viewer tests
    add_executable(test_consoleviewers test_consoleviewers.cpp)
    target_link_libraries(test_consoleviewers
        qvirt-console
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_consoleviewers COMMAND test_consoleviewers)

    # Wizard tests
    add_executable(test_wizards test_wizards.cpp)
    target_link_libraries(test_wizards
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_wizards COMMAND test_wizards)
else()
    message(STATUS "Qt${QT_VERSION_MAJOR}Test not found, unit tests disabled")
endif()
