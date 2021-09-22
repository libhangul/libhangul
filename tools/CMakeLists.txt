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

add_executable(tool-hangul
    hangul.c
)
target_compile_definitions(tool-hangul
    PRIVATE PACKAGE_NAME=\"${PROJECT_NAME}\"
    PRIVATE PRIVATE PACKAGE_VERSION=\"${PROJECT_VERSION}\"
)
set_target_properties(tool-hangul
    PROPERTIES OUTPUT_NAME hangul
)
target_link_libraries(tool-hangul
    LINK_PRIVATE hangul
)
