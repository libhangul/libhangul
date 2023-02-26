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

include(GNUInstallDirs)

if(ENABLE_EXTERNAL_KEYBOARDS)
    find_package(EXPAT)
endif()

set(hangul_PUBLIC_HEADERS
    hangul.h
)

set(hangul_PRIVATE_HEADERS
    hangul-gettext.h
    hangulkeyboard.h
    hangulinternals.h
    hanjacompatible.h
)

add_library(hangul
    ${hangul_PUBLIC_HEADERS}
    ${hangul_PRIVATE_HEADERS}
    hangulctype.c
    hangulinputcontext.c
    hangulkeyboard.c
    hanja.c
)

target_compile_definitions(hangul
    PRIVATE -DHAVE_CONFIG_H
    PRIVATE -DLOCALEDIR=\"${CMAKE_INSTALL_FULL_LOCALEDIR}\"
    PRIVATE -DLIBHANGUL_DEFAULT_HANJA_DIC=\"${CMAKE_INSTALL_FULL_DATADIR}/${CMAKE_PROJECT_NAME}/hanja/hanja.txt\"
    PRIVATE -DLIBHANGUL_DATA_DIR=\"${CMAKE_INSTALL_FULL_DATADIR}/${CMAKE_PROJECT_NAME}\"
    PRIVATE -DTOP_SRCDIR=\"${CMAKE_SOURCE_DIR}\"
)

target_include_directories(hangul
    PRIVATE "${CMAKE_BINARY_DIR}"
)

if(ENABLE_EXTERNAL_KEYBOARDS)
    target_compile_definitions(hangul
        PRIVATE -DENABLE_EXTERNAL_KEYBOARDS=1
    )

    target_include_directories(hangul
        PRIVATE ${EXPAT_INCLUDE_DIRS}
    )

    target_link_libraries(hangul LINK_PRIVATE
        ${EXPAT_LIBRARIES}
    )
endif() # ENABLE_EXTERNAL_KEYBOARDS

set_target_properties(hangul
    PROPERTIES
        VERSION "${LIBHANGUL_SOVERSION_MAJOR}.${LIBHANGUL_SOVERSION_MINOR}.${LIBHANGUL_SOVERSION_PATCH}"
        SOVERSION "${LIBHANGUL_SOVERSION_MAJOR}"
)

install(TARGETS hangul
    DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

install(FILES ${hangul_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR}/hangul-1.0
)
