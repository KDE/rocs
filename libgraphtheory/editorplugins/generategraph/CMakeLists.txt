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
add_library(generategraphplugin
    MODULE
    ${generategraph_SRCS}
)

target_link_libraries(generategraphplugin
    PUBLIC
    rocsgraphtheory
    KF5::Completion
)

ecm_optional_add_subdirectory(autotests)

install(TARGETS generategraphplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/editorplugins)
