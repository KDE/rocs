# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

file(COPY example.gml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(testgmlfileformat_SRCS
    testgmlfileformat.cpp
    ../gmlgrammar.cpp
    ../gmlgrammarhelper.cpp
    ../gmlfileformat.cpp
    ../../../logging.cpp
)

add_executable(TestGmlFileFormat ${testgmlfileformat_SRCS})
add_test(NAME TestGmlFileFormat COMMAND TestGmlFileFormat)
ecm_mark_as_test(TestGmlFileFormat)
target_link_libraries(TestGmlFileFormat
    rocsgraphtheory
    Qt5::Test
)

# Boost::spirit requires exceptions
kde_target_enable_exceptions(TestGmlFileFormat PRIVATE)
