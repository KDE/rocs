# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(tgfformat_SRCS
    tgffileformat.cpp
    ../../logging.cpp
)

add_library(tgffileformat MODULE ${tgfformat_SRCS})

target_link_libraries(tgffileformat
    rocsgraphtheory
)

install(TARGETS tgffileformat DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/fileformats)

ecm_optional_add_subdirectory(autotests)
