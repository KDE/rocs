##
#   This file is part of Rocs.
#   Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010-2012  Wagner Reck <wagner.reck@gmail.com>
#   Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of
#   the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

project(rocs)

add_definitions(-DTRANSLATION_DOMAIN=\"rocs\")

ecm_setup_version(1.9.900
    VARIABLE_PREFIX ROCS
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/rocsversion.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/RocsConfigVersion.cmake"
)

ecm_optional_add_subdirectory(plugins)

include_directories(
    ./
)

set( rocs_interface_SRCS
    ui/documentationwidget.cpp
    ui/journalwidget.cpp
    ui/sidedockwidget.cpp
    ui/fileformatdialog.cpp
    ui/mainwindow.cpp
    ui/codeeditorwidget.cpp
    ui/documenttypeswidget.cpp
    ui/grapheditorwidget.cpp
    ui/nodetypesdelegate.cpp
    ui/edgetypesdelegate.cpp
    ui/scriptoutputwidget.cpp
    project/project.cpp
#FIXME commented out, waiting for Grantlee Qt5 port
#     plugins/ApiDoc/ApiDocWidget.cpp
#     plugins/ApiDoc/ObjectDocumentation.cpp
#     plugins/ApiDoc/PropertyDocumentation.cpp
#     plugins/ApiDoc/MethodDocumentation.cpp
#     plugins/ApiDoc/ParameterDocumentation.cpp
#     plugins/ApiDoc/ApiDocManager.cpp
#     plugins/ApiDoc/ApiDocModel.cpp
)

set(
    rocs_SRCS
    main.cpp
    ${rocs_interface_SRCS}
    ${rocs_script_SRC}
)

ki18n_wrap_ui(
    rocs_SRCS
    ui/documentationwidget.ui
    ui/journalwidget.ui
    ui/scriptoutputwidget.ui
    plugins/ApiDoc/ApiDocWidget.ui
)

kconfig_add_kcfg_files(rocs_Settings_SRCS settings.kcfgc)

# needed to get icon on windows
# FIXME port to kf5
# kde4_add_app_icon( rocs_SRCS
#     ${CMAKE_CURRENT_SOURCE_DIR}/../graphics/global/hi*-apps-rocs.png
# )

add_executable( rocs
    ${rocs_SRCS}
    ${rocs_Settings_SRCS}
)

target_link_libraries(rocs
    PUBLIC
        rocsgraphtheory
        KF5::Archive
        KF5::CoreAddons
        KF5::Declarative
        KF5::I18n
        KF5::ItemViews
        KF5::Parts
        KF5::Service
        KF5::TextEditor
        Qt5::Core
        Qt5::Quick
        Qt5::QuickWidgets
        Qt5::WebKit
        Qt5::WebKitWidgets
        Qt5::XmlPatterns
)

################## INSTALLS ##########################

install(TARGETS rocs ${INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES rocsui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/rocs)
install(FILES rocs.kcfg DESTINATION ${KCFG_INSTALL_DIR})

ecm_optional_add_subdirectory(autotests)
