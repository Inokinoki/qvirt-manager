# Find Qt Test module once for all tests
if(BUILD_TESTING)
    find_package(Qt${QT_VERSION_MAJOR}Test ${QT_MIN_VERSION} QUIET)

    if(Qt${QT_VERSION_MAJOR}Test_FOUND)
        add_subdirectory(unit)
        add_subdirectory(integration)
    else()
        message(STATUS "Qt${QT_VERSION_MAJOR}Test not found, tests disabled")
    endif()
endif()
