# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(gmlformat_SRCS
    gmlfileformat.cpp
    gmlgrammar.cpp
    gmlgrammarhelper.cpp
    ../../logging.cpp
)

add_library(gmlfileformat MODULE ${gmlformat_SRCS})

target_link_libraries(gmlfileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

# Boost::spirit requires exceptions
kde_target_enable_exceptions(gmlfileformat PRIVATE)

install(TARGETS gmlfileformat DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/fileformats)

ecm_optional_add_subdirectory(autotests)
