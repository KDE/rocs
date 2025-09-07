# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

target_sources(rocsgraphtheory PRIVATE
    rocs2fileformat.cpp
    rocs2fileformat.h
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
