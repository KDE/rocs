# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause
# get generated *.json plugin file

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

file(COPY importtest_v1.graph2 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
add_executable(TestRocs2FileFormat
    testrocs2fileformat.cpp
    ../rocs2fileformat.cpp
    ../../../logging.cpp
)
add_test(NAME TestRocs2FileFormat COMMAND TestRocs2FileFormat)
ecm_mark_as_test(TestRocs2FileFormat)
target_link_libraries(TestRocs2FileFormat
    rocsgraphtheory
    Qt5::Test
)
