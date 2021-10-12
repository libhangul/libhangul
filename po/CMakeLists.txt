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

find_package(Gettext)
include(GNUInstallDirs)

file(READ LINGUAS LINGUAS)
string(REPLACE "\n" ";" LINGUAS "${LINGUAS}")

foreach(lang ${LINGUAS})
    GETTEXT_PROCESS_PO_FILES(${lang} ALL
        INSTALL_DESTINATION "${CMAKE_INSTALL_LOCALEDIR}"
        PO_FILES ${lang}.po
    )
endforeach()

find_program(XGETTEXT_EXECUTABLE xgettext)
set(PACKAGE_NAME libhangul)
set(DOMAIN libhangul)
set(COPYRIGHT_HOLDER "Choe Hwanjin")
set(MSGID_BUGS_ADDRESS "https://github.com/libhangul/libhangul")

add_custom_target(update-po)

add_custom_target(update-pot
    COMMAND
        ${XGETTEXT_EXECUTABLE}
            --default-domain=${DOMAIN}
            --directory="${CMAKE_SOURCE_DIR}"
            --add-comments=TRANSLATORS:
            --keyword=_
            --keyword=N_
            --files-from="${CMAKE_CURRENT_SOURCE_DIR}/POTFILES.in"
            --copyright-holder="${COPYRIGHT_HOLDER}"
            --package-name="${PACKAGE_NAME}"
            --package-version="${PACKAGE_VERSION}"
            --msgid-bugs-address="${MSGID_BUGS_ADDRESS}"
            --output-dir="${CMAKE_CURRENT_SOURCE_DIR}"
            --output=${DOMAIN}.pot
)
add_dependencies(update-po update-pot)

foreach(lang ${LINGUAS})
    add_custom_target(update-po-${lang}
        COMMAND "${GETTEXT_MSGMERGE_EXECUTABLE}" --update --lang=${lang} ${lang}.po ${DOMAIN}.pot
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${DOMAIN}.pot"
    )
    add_dependencies(update-po update-po-${lang})
endforeach()
