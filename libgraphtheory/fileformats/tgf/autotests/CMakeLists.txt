# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause
# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
add_executable(TestTgfFileFormat
    testtgffileformat.cpp
    ../tgffileformat.cpp
    ../../../logging.cpp
)
add_test(NAME TestTgfFileFormat COMMAND TestTgfFileFormat)
ecm_mark_as_test(TestTgfFileFormat)
target_link_libraries(TestTgfFileFormat
    rocsgraphtheory
    Qt5::Test
)
