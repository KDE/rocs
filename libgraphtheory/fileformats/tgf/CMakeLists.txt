# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(tgfformat_SRCS
    tgffileformat.cpp
    tgffileformat.h
    ../../logging.cpp
    tgffileformat.json
)

kcoreaddons_add_plugin(tgffileformat SOURCES ${tgfformat_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(tgffileformat
    rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
