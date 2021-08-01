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

option(ENABLE_EXTERNAL_KEYBOARDS
    "enable external keyboard xml file loading feature"
    ON
)

include(CTest)

include(GNUInstallDirs)
set(LIBHANGUL_INCLUDE_DIR "${CMAKE_INSTALL_INCLUDEDIR}/hangul-1.0")
set(LIBHANGUL_LIBRARY_DIR "${CMAKE_INSTALL_LIBDIR}")

add_subdirectory(hangul)
add_subdirectory(data/hanja)
add_subdirectory(data/keyboards)
add_subdirectory(po)

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(hangul-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/hangul-config.cmake"
    INSTALL_DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/hangul"
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
        "${CMAKE_INSTALL_LIBDIR}/cmake/hangul"
    COMPONENT
        dev
)
