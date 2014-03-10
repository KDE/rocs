##
#   This file is part of Rocs.
#   Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(TestKmlFileFormatPlugin_SRCS
    KmlFileFormatTest.cpp
    ../KmlHandler.cpp
    ../KmlFileFormatPlugin.cpp
)
add_executable(TestKmlFileFormatPlugin ${TestKmlFileFormatPlugin_SRCS})
add_test(TestKmlFileFormatPlugin TestKmlFileFormatPlugin)
ecm_mark_as_test(TestKmlFileFormatPlugin)
target_link_libraries(TestKmlFileFormatPlugin
    rocscore
    Qt5::Test
)
