# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_executable(scenetest
    scenetest.cpp
)

target_link_libraries(scenetest
    PUBLIC
        rocsgraphtheory
        Qt5::Core
        Qt5::Quick
        Qt5::Widgets
        KF5::I18n
)
