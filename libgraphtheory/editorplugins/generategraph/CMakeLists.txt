# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

# Boost requires exceptions for this plugin
kde_enable_exceptions()

set(generategraph_SRCS
    generategraphplugin.cpp
    generategraphwidget.cpp
    ../../logging.cpp
)

#boost requires exceptions
kde_source_files_enable_exceptions(generategraphplugin.cpp)

ki18n_wrap_ui(generategraph_SRCS generategraphwidget.ui)
kcoreaddons_add_plugin(generategraphplugin SOURCES ${generategraph_SRCS} INSTALL_NAMESPACE "rocs/editorplugins")

target_link_libraries(generategraphplugin
    PUBLIC
    rocsgraphtheory
    KF6::Completion
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
