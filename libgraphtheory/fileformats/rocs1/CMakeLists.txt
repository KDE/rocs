# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

target_sources(rocsgraphtheory PRIVATE
    rocs1fileformat.cpp
    rocs1fileformat.h
)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()
