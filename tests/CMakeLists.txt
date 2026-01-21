# Unit tests
if(BUILD_TESTING)
    find_package(Qt${QT_VERSION_MAJOR}Test ${QT_MIN_VERSION} QUIET)

    if(Qt${QT_VERSION_MAJOR}Test_FOUND)
        add_subdirectory(unit)
    else()
        message(STATUS "Qt${QT_VERSION_MAJOR}Test not found, unit tests disabled")
    endif()
endif()

# Integration tests
if(BUILD_TESTING)
    add_subdirectory(integration)
endif()
