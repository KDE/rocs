# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

# get generated *.json plugin file
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})
set(testrocs1fileformat_SRCS
    testrocs1fileformat.cpp
    ../rocs1fileformat.cpp
    ../../../logging.cpp
)
add_executable(TestRocs1FileFormat ${testrocs1fileformat_SRCS})
add_test(NAME TestRocs1FileFormat COMMAND TestRocs1FileFormat)
ecm_mark_as_test(TestRocs1FileFormat)
target_link_libraries(TestRocs1FileFormat
    rocsgraphtheory
    Qt5::Test
)
