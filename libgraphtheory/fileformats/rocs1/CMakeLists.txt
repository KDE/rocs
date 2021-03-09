# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs1format_SRCS
    rocs1fileformat.cpp
    ../../logging.cpp
)

add_library(rocs1fileformat MODULE ${rocs1format_SRCS})

target_link_libraries(rocs1fileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

install(TARGETS rocs1fileformat DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/fileformats)

ecm_optional_add_subdirectory(autotests)
