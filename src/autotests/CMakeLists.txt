# SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

#file(COPY testfiles DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TestProject
    test_project.cpp
    test_project.h
    ../project/project.cpp
)
add_test(NAME TestProject COMMAND TestProject)
target_compile_definitions(TestProject PRIVATE ADDITIONAL_PLUGIN_DIR="${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
ecm_mark_as_test(TestProject)
target_link_libraries(TestProject
    rocsgraphtheory
    KF6::TextEditor
    KF6::Archive
    Qt::Test
)
