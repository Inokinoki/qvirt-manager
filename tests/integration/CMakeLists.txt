# Integration tests
# Note: Qt${QT_VERSION_MAJOR}Test is already found in parent tests/CMakeLists.txt

# Connection lifecycle tests
add_executable(test_connection_lifecycle test_connection_lifecycle.cpp)
target_link_libraries(test_connection_lifecycle
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_connection_lifecycle PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_connection_lifecycle COMMAND test_connection_lifecycle)

# VM operations tests (requires libvirt)
add_executable(test_vm_operations test_vm_operations.cpp)
target_link_libraries(test_vm_operations
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_vm_operations PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_vm_operations COMMAND test_vm_operations)

# UI workflow tests
add_executable(test_ui_workflow test_ui_workflow.cpp)
target_link_libraries(test_ui_workflow
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_ui_workflow PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_ui_workflow COMMAND test_ui_workflow)

# Async connection tests
add_executable(test_async_connection test_async_connection.cpp)
target_link_libraries(test_async_connection
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_async_connection PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_async_connection COMMAND test_async_connection)

# ManagerWindow UI tests
add_executable(test_manager_window test_manager_window.cpp)
target_link_libraries(test_manager_window
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_manager_window PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_manager_window COMMAND test_manager_window)

# VMWindow UI tests
add_executable(test_vm_window test_vm_window.cpp)
target_link_libraries(test_vm_window
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_vm_window PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_vm_window COMMAND test_vm_window)

# CreateVMWizard UI tests
add_executable(test_createvm_wizard test_createvm_wizard.cpp)
target_link_libraries(test_createvm_wizard
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_createvm_wizard PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_createvm_wizard COMMAND test_createvm_wizard)

# Performance benchmark tests
add_executable(test_performance_benchmarks test_performance_benchmarks.cpp)
target_link_libraries(test_performance_benchmarks
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_performance_benchmarks PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_performance_benchmarks COMMAND test_performance_benchmarks)

# Dialog tests (CloneDialog, SnapshotDialog, etc.)
add_executable(test_dialogs test_dialogs.cpp)
target_link_libraries(test_dialogs
    qvirt-ui
    qvirt-libvirt
    qvirt-core
    Qt${QT_VERSION_MAJOR}::Test
    ${LIBVIRT_LIBRARIES}
)
target_link_directories(test_dialogs PRIVATE ${LIBVIRT_LIBRARY_DIRS})
add_test(NAME test_dialogs COMMAND test_dialogs)
