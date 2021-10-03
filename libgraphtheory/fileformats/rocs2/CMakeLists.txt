# SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(rocs2format_SRCS
    rocs2fileformat.cpp
    ../../logging.cpp
)

kcoreaddons_add_plugin(rocs2fileformat SOURCES ${rocs2format_SRCS} INSTALL_NAMESPACE "rocs/fileformats")

target_link_libraries(rocs2fileformat
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::I18n
        rocsgraphtheory
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
