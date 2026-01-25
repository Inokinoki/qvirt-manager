cmake_minimum_required(VERSION 3.16)
project(qvirt-manager VERSION 1.0.0 LANGUAGES CXX)

# Qt version selection
set(QT_VERSION_MAJOR "6" CACHE STRING "Qt major version to use (5 or 6)")

# Setup Qt
if(QT_VERSION_MAJOR EQUAL 6)
    set(QT_MIN_VERSION "6.2.0")
    set(QT_COMPONENTS Core Widgets Network Xml UiTools)
    set(QT_OPTIONAL_COMPONENTS SerialPort)
elseif(QT_VERSION_MAJOR EQUAL 5)
    set(QT_MIN_VERSION "5.9.0")
    set(QT_COMPONENTS Core Widgets Network Xml UiTools)
    set(QT_OPTIONAL_COMPONENTS SerialPort)
else()
    message(FATAL_ERROR "Qt version must be 5 or 6, got: ${QT_VERSION_MAJOR}")
endif()

find_package(Qt${QT_VERSION_MAJOR} ${QT_MIN_VERSION} REQUIRED COMPONENTS ${QT_COMPONENTS})

# Standard C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Compiler settings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find libvirt (optional, can be disabled with ENABLE_LIBVIRT=OFF)
option(ENABLE_LIBVIRT "Enable libvirt support" ON)
if(ENABLE_LIBVIRT)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(LIBVIRT libvirt)
        if(NOT LIBVIRT_FOUND)
            message(WARNING "libvirt not found, building without libvirt support")
        endif()
    else()
        message(WARNING "PkgConfig not found, cannot check for libvirt")
    endif()
else()
    message(STATUS "libvirt support disabled by ENABLE_LIBVIRT=OFF")
endif()

# Optional dependencies
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SPICEGTK spice-client-gtk-2.0)
    pkg_check_modules(GTKVNC vnc-1.0)
endif()

# Subdirectories
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(tests)

# Installation
include(GNUInstallDirs)
install(TARGETS qvirt-manager DESTINATION ${CMAKE_INSTALL_BINDIR})

# Print configuration summary
message(STATUS "")
message(STATUS "qvirt-manager configuration:")
message(STATUS "  Qt version: ${QT_VERSION_MAJOR}")
message(STATUS "  Qt components: ${QT_COMPONENTS}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
if(LIBVIRT_FOUND)
    message(STATUS "  libvirt: ${LIBVIRT_VERSION} (${LIBVIRT_INCLUDE_DIRS})")
else()
    message(STATUS "  libvirt: NOT FOUND (building without libvirt support)")
endif()
if(SPICEGTK_FOUND)
    message(STATUS "  Spice GTK: ${SPICEGTK_VERSION} (optional)")
endif()
if(GTKVNC_FOUND)
    message(STATUS "  GtkVNC: ${GTKVNC_VERSION} (optional)")
endif()
message(STATUS "")
