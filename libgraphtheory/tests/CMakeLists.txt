# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

add_executable(scenetest
    scenetest.cpp
)

target_link_libraries(scenetest
    PUBLIC
        rocsgraphtheory
        Qt::Core
        Qt::Quick
        Qt::Widgets
        KF5::I18n
)
