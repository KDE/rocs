# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(tikzformat_SRCS
    tikzfileformat.cpp
    tikzfileformat.h
    ../../logging.cpp
    tikzfileformat.json
)

kcoreaddons_add_plugin(tikzfileformat SOURCES ${tikzformat_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(tikzfileformat
    rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
