# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(gmlformat_SRCS
    gmlfileformat.cpp
    gmlfileformat.h
    gmlgrammar.cpp
    gmlgrammar.h
    gmlgrammarhelper.cpp
    ../../logging.cpp
    gmlfileformat.json
)

kcoreaddons_add_plugin(gmlfileformat SOURCES ${gmlformat_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(gmlfileformat
    PUBLIC
        Qt::Core
        Qt::Gui
        KF6::I18n
        rocsgraphtheory
)

# Boost::spirit requires exceptions
kde_target_enable_exceptions(gmlfileformat PRIVATE)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
