# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(transformedges_SRCS
    transformedgesplugin.cpp
    transformedgeswidget.cpp
    ../../logging.cpp
)
ki18n_wrap_ui(transformedges_SRCS transformedgeswidget.ui)

kcoreaddons_add_plugin(transformedgesplugin SOURCES ${transformedges_SRCS} INSTALL_NAMESPACE "rocs/editorplugins")

target_link_libraries(transformedgesplugin
    PUBLIC
    rocsgraphtheory
    KF6::Completion
)

if(BUILD_TESTING)
    ecm_optional_add_subdirectory(autotests)
endif()
