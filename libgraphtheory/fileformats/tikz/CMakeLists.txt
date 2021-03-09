# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(tikzformat_SRCS
    tikzfileformat.cpp
    ../../logging.cpp
)

add_library(tikzfileformat MODULE ${tikzformat_SRCS})

target_link_libraries(tikzfileformat
    rocsgraphtheory
)

install(TARGETS tikzfileformat DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/fileformats)

ecm_optional_add_subdirectory(autotests)
