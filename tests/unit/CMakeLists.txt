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

    # SystemTray tests
    add_executable(test_systemtray test_systemtray.cpp)
    target_link_libraries(test_systemtray
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_systemtray COMMAND test_systemtray)

    # ProgressDialog tests
    add_executable(test_progressdialog test_progressdialog.cpp)
    target_link_libraries(test_progressdialog
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_progressdialog COMMAND test_progressdialog)

    # ManagerWindow UI behavior tests
    add_executable(test_managerwindow test_managerwindow.cpp)
    target_link_libraries(test_managerwindow
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_managerwindow COMMAND test_managerwindow)

    # ContextMenu UI behavior tests
    add_executable(test_contextmenu test_contextmenu.cpp)
    target_link_libraries(test_contextmenu
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_contextmenu COMMAND test_contextmenu)

    # CreateVMWizard workflow tests
    add_executable(test_createvmwizard test_createvmwizard.cpp)
    target_link_libraries(test_createvmwizard
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_createvmwizard COMMAND test_createvmwizard)

    # Dialog behaviors tests
    add_executable(test_dialogbehaviors test_dialogbehaviors.cpp)
    target_link_libraries(test_dialogbehaviors
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_dialogbehaviors COMMAND test_dialogbehaviors)

    # Keyboard shortcuts and SystemTray tests
    add_executable(test_keyboardshortcuts test_keyboardshortcuts.cpp)
    target_link_libraries(test_keyboardshortcuts
        qvirt-ui
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_keyboardshortcuts COMMAND test_keyboardshortcuts)

    # VMWindow tests
    add_executable(test_vmwindow test_vmwindow.cpp)
    target_link_libraries(test_vmwindow
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_vmwindow COMMAND test_vmwindow)

    # Storage and Network dialog tests
    add_executable(test_storageandnetwork test_storageandnetwork.cpp)
    target_link_libraries(test_storageandnetwork
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_storageandnetwork COMMAND test_storageandnetwork)

    # UI Models tests
    add_executable(test_uimodels test_uimodels.cpp)
    target_link_libraries(test_uimodels
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_uimodels COMMAND test_uimodels)

    # XMLEditor tests
    add_executable(test_xmleditor test_xmleditor.cpp)
    target_link_libraries(test_xmleditor
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_xmleditor COMMAND test_xmleditor)

    # AddHardwareDialog tests
    add_executable(test_addhardwaredialog test_addhardwaredialog.cpp)
    target_link_libraries(test_addhardwaredialog
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_addhardwaredialog COMMAND test_addhardwaredialog)

    # VM Page tests
    add_executable(test_vmpagedtests test_vmpagedtests.cpp)
    target_link_libraries(test_vmpagedtests
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_vmpagedtests COMMAND test_vmpagedtests)

    # SnapshotsPage tests
    add_executable(test_snapshots test_snapshots.cpp)
    target_link_libraries(test_snapshots
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_snapshots COMMAND test_snapshots)

    # ConsolePage tests
    add_executable(test_console test_console.cpp)
    target_link_libraries(test_console
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_console COMMAND test_console)

    # PreferencesDialog tests
    add_executable(test_preferences test_preferences.cpp)
    target_link_libraries(test_preferences
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_preferences COMMAND test_preferences)
else()
    message(STATUS "Qt${QT_VERSION_MAJOR}Test not found, unit tests disabled")
endif()
