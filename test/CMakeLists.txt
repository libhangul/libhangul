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

find_package(PkgConfig)

add_executable(test-hangul
    hangul.c
)
target_compile_definitions(test-hangul PRIVATE
    TEST_LIBHANGUL_KEYBOARD_PATH=\"${CMAKE_BINARY_DIR}/data/keyboards\"
)
target_link_libraries(test-hangul LINK_PRIVATE hangul)

add_executable(test-hanja
    hanja.c
)
target_compile_definitions(test-hanja PRIVATE
    TEST_HANJA_TXT=\"${CMAKE_SOURCE_DIR}/data/hanja/hanja.txt\"
)
target_link_libraries(test-hanja LINK_PRIVATE hangul)

# unit test
if(ENABLE_UNIT_TEST)

pkg_check_modules(CHECK REQUIRED check)

add_executable(unittest
    test.c
)

target_compile_definitions(unittest PRIVATE
    TEST_LIBHANGUL_KEYBOARD_PATH=\"${CMAKE_BINARY_DIR}/data/keyboards\"
    TEST_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"
)
target_include_directories(unittest PRIVATE ${CHECK_INCLUDE_DIRS})
target_link_libraries(unittest PRIVATE hangul ${CHECK_LDFLAGS})

add_test(NAME unittest
    COMMAND ./unittest
)

endif() # ENABLE_UNIT_TEST)
