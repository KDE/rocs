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

set(ROCS_VERSION_MAJOR 1)
set(ROCS_VERSION_MINOR 9)
set(ROCS_VERSION_PATCH 80)

configure_file( "${rocs_SOURCE_DIR}/rocsversion.h.cmake" "${rocs_BINARY_DIR}/rocsversion.h" @ONLY )

add_subdirectory(InterfacePlugins)

include_directories(
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
    Actions/
    GraphicsItem/
    Ui/
    LoadSave/
    Plugins/
    Scripts/
)

set(rocs_actions_SRCS
    Actions/PropertiesDialogAction.cpp
)

set( rocs_interface_SRCS
    Ui/DocumentationWidget.cpp
    Ui/JournalEditorWidget.cpp
    Ui/SideDockWidget.cpp
    Ui/ImporterExporterManager.cpp
    Ui/IncludeManagerSettings.cpp
    Ui/MainWindow.cpp
    Ui/Separator.cpp
    Ui/TabWidget.cpp
    Ui/CodeEditor.cpp
    Ui/DocumentTypesWidget.cpp
    Ui/ConfigureDefaultProperties.cpp
#     Ui/LoadedPluginsDialog.cpp //FIXME
    Ui/PossibleIncludes.cpp
    Ui/ScriptOutputWidget.cpp
#FIXME commented out, waiting for Grantlee Qt5 port
#     InterfacePlugins/ApiDoc/ApiDocWidget.cpp
#     InterfacePlugins/ApiDoc/ObjectDocumentation.cpp
#     InterfacePlugins/ApiDoc/PropertyDocumentation.cpp
#     InterfacePlugins/ApiDoc/MethodDocumentation.cpp
#     InterfacePlugins/ApiDoc/ParameterDocumentation.cpp
#     InterfacePlugins/ApiDoc/ApiDocManager.cpp
#     InterfacePlugins/ApiDoc/ApiDocModel.cpp
)

set(
    rocs_SRCS
    Main.cpp
    ${rocs_interface_SRCS}
    ${rocs_script_SRC}
)

qt5_wrap_ui(
    rocs_SRCS
    Ui/DocumentationWidget.ui
    Ui/JournalEditorWidget.ui
    Ui/DocumentTypesWidget.ui
    Ui/ConfigureDefaultProperties.ui
    Ui/ScriptOutputWidget.ui
    InterfacePlugins/ApiDoc/ApiDocWidget.ui
)

# we currently only support QtScript
set(QT_USE_QTSCRIPT true)

kconfig_add_kcfg_files(rocs_Settings_SRCS settings.kcfgc)

# needed to get icon on windows
#TODO port to kf5
kde4_add_app_icon( rocs_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphics/global/hi*-apps-rocs.png
)

add_executable( rocs
    ${rocs_SRCS}
    ${rocs_Settings_SRCS}
    ${rocs_RCC_SRCS}
)

target_link_libraries(rocs
    PUBLIC
        rocscore
        rocsvisualeditor
        ${Qt5WebKit_LIBRARIES}
        ${Qt5WebKitWidgets_LIBRARIES}
        ${Qt5XmlPatterns_LIBRARIES}
        KF5::TextEditor
        KF5::Archive
        KF5::CoreAddons
        KF5::KDE4Support
        KF5::Service
        KF5::I18n
        KF5::NewStuff
        KF5::ItemViews
)

################## INSTALLS ##########################

install( TARGETS rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( PROGRAMS rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs )
install( FILES rocs.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES rocs.knsrc DESTINATION ${CONFIG_INSTALL_DIR} )

add_subdirectory( Scripts )

# Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
