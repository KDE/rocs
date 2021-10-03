# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
add_executable(TestTikzFileFormat
    testtikzfileformat.cpp
    ../tikzfileformat.cpp
    ../../../logging.cpp
)
add_test(NAME TestTikzFileFormat COMMAND TestTikzFileFormat)
ecm_mark_as_test(TestTikzFileFormat)
target_link_libraries(TestTikzFileFormat
    rocsgraphtheory
    Qt5::Test
)
