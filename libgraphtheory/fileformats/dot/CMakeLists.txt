# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(dotformat_SRCS
    dotfileformat.cpp
    dotgrammar.cpp
    dotgrammarhelper.cpp
    ../../logging.cpp
)

# Boost requires exceptions
kde_source_files_enable_exceptions(dotgrammar.cpp)

kcoreaddons_add_plugin(dotfileformat SOURCES ${dotformat_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(dotfileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

ecm_optional_add_subdirectory(autotests)
