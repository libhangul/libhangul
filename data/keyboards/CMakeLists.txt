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

cmake_minimum_required(VERSION 3.2)

#include(GNUInstallDirs)
find_package(Gettext)

set(keyboard_file_list
    hangul-keyboard-2.xml
    hangul-keyboard-2y.xml
    hangul-keyboard-39.xml
    hangul-keyboard-3f.xml
    hangul-keyboard-32.xml
    hangul-keyboard-3s.xml
    hangul-keyboard-3y.xml
    hangul-keyboard-ro.xml
    hangul-keyboard-ahn.xml
)

add_custom_target(keyboard ALL
    DEPENDS
        ${keyboard_file_list}
        hangul-combination-default.xml
        hangul-combination-full.xml
)

set(MSGFMT_COMMAND env GETTEXTDATADIRS=\"${CMAKE_CURRENT_SOURCE_DIR}\" \"${GETTEXT_MSGFMT_EXECUTABLE}\")

# generating keyboard translation rule
foreach(i ${keyboard_file_list})
    string(REGEX REPLACE "\\.xml$" "" keyboard_name "${i}")
    add_custom_command(
        OUTPUT
            "${keyboard_name}.name.xml.in"
        COMMAND
            grep \"xml version=\" "${CMAKE_CURRENT_SOURCE_DIR}/${keyboard_name}.xml.template" > "${keyboard_name}.name.xml.in"
        COMMAND
            grep \"<hangul-keyboard\" "${CMAKE_CURRENT_SOURCE_DIR}/${keyboard_name}.xml.template" >> "${keyboard_name}.name.xml.in"
        COMMAND
            grep \"<name>\" "${CMAKE_CURRENT_SOURCE_DIR}/${keyboard_name}.xml.template" >> "${keyboard_name}.name.xml.in"
        COMMAND
            grep \"</hangul-keyboard\" "${CMAKE_CURRENT_SOURCE_DIR}/${keyboard_name}.xml.template" >> "${keyboard_name}.name.xml.in"
        DEPENDS
            "${keyboard_name}.xml.template"
    )

    add_custom_command(
        OUTPUT
            "${keyboard_name}.name.xml"
        COMMAND
            ${MSGFMT_COMMAND} --xml --template "${keyboard_name}.name.xml.in" -d "${CMAKE_SOURCE_DIR}/po" -o "${keyboard_name}.name.xml"
        DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/${keyboard_name}.name.xml.in"
    )

    add_custom_command(
        OUTPUT
            "${keyboard_name}.xml"
        COMMAND
            sed -i.tmp -e '1 D'
                    -e '/^<hangul-keyboard/D'
                    -e '/^<\\/hangul-keyboard/D'
                    -e 's/^ *<name/    <name/' "${keyboard_name}.name.xml"
        COMMAND
            sed
                    -e '/<name>/r ${keyboard_name}.name.xml'
                    -e '/<name>/D'
                    "${CMAKE_CURRENT_SOURCE_DIR}/${keyboard_name}.xml.template" > "${keyboard_name}.xml"
        BYPRODUCTS
            "${keyboard_name}.name.xml.tmp"
        DEPENDS
            "${CMAKE_CURRENT_BINARY_DIR}/${keyboard_name}.name.xml"
            "${keyboard_name}.xml.template"
    )

    install(
        FILES "${CMAKE_CURRENT_BINARY_DIR}/${keyboard_name}.xml"
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/keyboards"
    )
endforeach()

add_custom_command(
    OUTPUT
        hangul-combination-default.xml
    COMMAND
        "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/hangul-combination-default.xml" hangul-combination-default.xml
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/hangul-combination-default.xml"
)

add_custom_command(
    OUTPUT
        hangul-combination-full.xml
    COMMAND
        "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/hangul-combination-full.xml" hangul-combination-full.xml
    DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/hangul-combination-full.xml"
)

install(
    FILES
        hangul-combination-default.xml
        hangul-combination-full.xml
    DESTINATION
        "${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/keyboards"
)
