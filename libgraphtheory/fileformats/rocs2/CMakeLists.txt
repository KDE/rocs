# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs2format_SRCS
    rocs2fileformat.cpp
    rocs2fileformat.h
    ../../logging.cpp
    rocs2fileformat.json
)

kcoreaddons_add_plugin(rocs2fileformat SOURCES ${rocs2format_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(rocs2fileformat
    PUBLIC
        Qt::Core
        Qt::Gui
        KF6::I18n
        rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
