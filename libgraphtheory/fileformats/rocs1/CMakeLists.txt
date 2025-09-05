# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs1format_SRCS
    rocs1fileformat.cpp
    rocs1fileformat.h
    ../../logging.cpp
    rocs1fileformat.json
)

kcoreaddons_add_plugin(rocs1fileformat SOURCES ${rocs1format_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(rocs1fileformat
    PUBLIC
        Qt::Core
        Qt::Gui
        KF6::I18n
        rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
