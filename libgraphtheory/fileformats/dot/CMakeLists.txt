# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

target_sources(rocsgraphtheory PRIVATE
    dotfileformat.cpp
    dotfileformat.h
    dotgrammar.cpp
    dotgrammar.h
    dotgrammarhelper.cpp
    dotgrammarhelper.h
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
