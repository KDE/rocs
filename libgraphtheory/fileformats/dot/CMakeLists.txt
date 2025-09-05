# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(dotformat_SRCS
    dotfileformat.cpp
    dotfileformat.h
    dotgrammar.cpp
    dotgrammar.h
    dotgrammarhelper.cpp
    dotgrammarhelper.h
    ../../logging.cpp
    dotfileformat.json
)

# Boost requires exceptions
kde_source_files_enable_exceptions(dotgrammar.cpp)

kcoreaddons_add_plugin(dotfileformat SOURCES ${dotformat_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(dotfileformat
    PUBLIC
        Qt::Core
        Qt::Gui
        KF6::I18n
        rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
