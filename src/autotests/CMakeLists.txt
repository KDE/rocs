# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

file(COPY testfiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(testproject_SRCS
    test_project.cpp
    ../project/project.cpp
)
add_executable(TestProject ${testproject_SRCS})
add_test(NAME TestProject COMMAND TestProject)
ecm_mark_as_test(TestProject)
target_link_libraries(TestProject
    rocsgraphtheory
    KF5::TextEditor
    KF5::Archive
    Qt5::Test
)
