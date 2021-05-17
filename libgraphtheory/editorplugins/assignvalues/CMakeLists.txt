# SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

set(assignvalues_SRCS
    assignvaluesplugin.cpp
    assignvalueswidget.cpp
    ../../logging.cpp
)
ki18n_wrap_ui(assignvalues_SRCS assignvalueswidget.ui)
kcoreaddons_add_plugin(assignvaluesplugin SOURCES ${assignvalues_SRCS} INSTALL_NAMESPACE "rocs/editorplugins")

target_link_libraries(assignvaluesplugin
    PUBLIC
    rocsgraphtheory
    KF5::Completion
)

kde_source_files_enable_exceptions(assignvalueswidget.cpp)
ecm_optional_add_subdirectory(autotests)
