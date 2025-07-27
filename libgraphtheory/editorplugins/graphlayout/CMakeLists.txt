# SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
#
# SPDX-License-Identifier: BSD-2-Clause

# Boost requires exceptions for this plugin
kde_enable_exceptions()

add_library(graphlayoutplugin MODULE)

target_sources(graphlayoutplugin PRIVATE
    graphlayoutplugin.cpp
    graphlayoutwidget.cpp
    ../../logging.cpp
)

#boost requires exceptions
kde_source_files_enable_exceptions(graphlayoutplugin.cpp)

ki18n_wrap_ui(graphlayoutplugin graphlayoutwidget.ui)

target_link_libraries(graphlayoutplugin
    PUBLIC
    rocsgraphtheory
    KF6::Completion
)

install(TARGETS graphlayoutplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/editorplugins)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
