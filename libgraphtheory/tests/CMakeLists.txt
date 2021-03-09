# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(scenetest_SRCS
    scenetest.cpp
)

add_executable(
    scenetest
    ${scenetest_SRCS}
)

target_link_libraries(scenetest
    PUBLIC
        rocsgraphtheory
        Qt5::Core
        Qt5::Quick
        Qt5::Widgets
        KF5::I18n
)
