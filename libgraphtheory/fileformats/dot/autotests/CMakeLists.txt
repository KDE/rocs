# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause


# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

file(COPY testfiles/directed DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY testfiles/undirected DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(testdotfileformat_SRCS
    testdotfileformat.cpp
    ../dotfileformat.cpp
    ../dotgrammar.cpp
    ../dotgrammarhelper.cpp
    ../../../logging.cpp
)
kde_source_files_enable_exceptions(../dotgrammar.cpp)
add_executable(TestDotFileFormat ${testdotfileformat_SRCS})
add_test(NAME TestDotFileFormat COMMAND TestDotFileFormat)
ecm_mark_as_test(TestDotFileFormat)
target_link_libraries(TestDotFileFormat
    rocsgraphtheory
    Qt5::Gui
    Qt5::Test
)
