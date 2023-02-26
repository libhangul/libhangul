# libhangul
# Copyright 2021 Choe Hwanjin
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

cmake_minimum_required(VERSION 3.0)

project(libhangul
    VERSION 0.1.0
)

set(LIBHANGUL_SOVERSION_MAJOR 1)
set(LIBHANGUL_SOVERSION_MINOR 0)
set(LIBHANGUL_SOVERSION_PATCH 1)

option(BUILD_SHARED_LIBS
    "Enable/Disable shared library build."
    ON
)

option(ENABLE_EXTERNAL_KEYBOARDS
    "Enable/Disable external keyboard xml file loading feature."
    ON
)

option(ENABLE_UNIT_TEST
    "Enable/Disable unit test."
    ON
)

include(CTest)

include(GNUInstallDirs)
include(CheckIncludeFiles)
set(LIBHANGUL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/hangul-1.0")
set(LIBHANGUL_LIBRARY_DIR "${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(hangul)
add_subdirectory(data/hanja)
add_subdirectory(data/keyboards)
add_subdirectory(tools)
add_subdirectory(po)

if(BUILD_TESTING)
    add_subdirectory(test EXCLUDE_FROM_ALL)
endif()

check_include_files(glob.h HAVE_GLOB_H)
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

include(CMakePackageConfigHelpers)
configure_package_config_file(hangul-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/hangul-config.cmake"
    INSTALL_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
    PATH_VARS
        LIBHANGUL_INCLUDE_DIR
        LIBHANGUL_LIBRARY_DIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/hangul-config-version.cmake"
    COMPATIBILITY
        AnyNewerVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/hangul-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/hangul-config-version.cmake"
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}"
    COMPONENT
        dev
)
