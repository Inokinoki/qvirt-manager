# Integration tests
find_package(Qt${QT_VERSION_MAJOR}Test ${QT_MIN_VERSION} QUIET)

if(Qt${QT_VERSION_MAJOR}Test_FOUND)
    # Connection lifecycle tests
    add_executable(test_connection_lifecycle test_connection_lifecycle.cpp)
    target_link_libraries(test_connection_lifecycle
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_connection_lifecycle COMMAND test_connection_lifecycle)

    # VM operations tests (requires libvirt)
    add_executable(test_vm_operations test_vm_operations.cpp)
    target_link_libraries(test_vm_operations
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_vm_operations COMMAND test_vm_operations)

    # UI workflow tests
    add_executable(test_ui_workflow test_ui_workflow.cpp)
    target_link_libraries(test_ui_workflow
        qvirt-ui
        qvirt-libvirt
        qvirt-core
        Qt${QT_VERSION_MAJOR}::Test
    )
    add_test(NAME test_ui_workflow COMMAND test_ui_workflow)
else()
    message(STATUS "Qt${QT_VERSION_MAJOR}Test not found, integration tests disabled")
endif()
