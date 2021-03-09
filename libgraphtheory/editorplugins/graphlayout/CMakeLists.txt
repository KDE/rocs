# SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

# Boost requires exceptions for this plugin
kde_enable_exceptions()

set(graphlayout_SRCS
    graphlayoutplugin.cpp
    graphlayoutwidget.cpp
    ../../logging.cpp
)

#boost requires exceptions
kde_source_files_enable_exceptions(graphlayoutplugin.cpp)

ki18n_wrap_ui(graphlayout_SRCS graphlayoutwidget.ui)
add_library(graphlayoutplugin
    MODULE
    ${graphlayout_SRCS}
)

target_link_libraries(graphlayoutplugin
    PUBLIC
    rocsgraphtheory
    KF5::Completion
)

ecm_optional_add_subdirectory(autotests)

install(TARGETS graphlayoutplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/editorplugins)
