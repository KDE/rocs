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

# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

file(COPY example.gml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(testgmlfileformat_SRCS
    testgmlfileformat.cpp
    ../gmlgrammar.cpp
    ../gmlgrammarhelper.cpp
    ../gmlfileformat.cpp
)
add_executable(TestGmlFileFormat ${testgmlfileformat_SRCS})
add_test(TestGmlFileFormat TestGmlFileFormat)
ecm_mark_as_test(TestGmlFileForma)
target_link_libraries(TestGmlFileFormat
    rocscore
    Qt5::Test
)
