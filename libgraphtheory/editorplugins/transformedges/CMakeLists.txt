# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(transformedges_SRCS
    transformedgesplugin.cpp
    transformedgeswidget.cpp
    ../../logging.cpp
)
ki18n_wrap_ui(transformedges_SRCS transformedgeswidget.ui)
add_library(transformedgesplugin
    MODULE
    ${transformedges_SRCS}
)

target_link_libraries(transformedgesplugin
    PUBLIC
    rocsgraphtheory
    KF5::Completion
)

ecm_optional_add_subdirectory(autotests)

install(TARGETS transformedgesplugin DESTINATION ${KDE_INSTALL_PLUGINDIR}/rocs/editorplugins)
