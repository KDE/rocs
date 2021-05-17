# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs1format_SRCS
    rocs1fileformat.cpp
    ../../logging.cpp
)

kcoreaddons_add_plugin(rocs1fileformat SOURCES ${rocs1format_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(rocs1fileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

ecm_optional_add_subdirectory(autotests)
