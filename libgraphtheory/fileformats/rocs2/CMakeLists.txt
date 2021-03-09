# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs2format_SRCS
    rocs2fileformat.cpp
    ../../logging.cpp
)

add_library(rocs2fileformat MODULE ${rocs2format_SRCS})

target_link_libraries(rocs2fileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

install(TARGETS rocs2fileformat DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/fileformats)

ecm_optional_add_subdirectory(autotests)
