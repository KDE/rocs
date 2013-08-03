##
#   This file is part of Rocs.
#   Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
#   Copyright 2010-2012  Wagner Reck <wagner.reck@gmail.com>
#   Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
set(ROCS_VERSION_MINOR 8)
set(ROCS_VERSION_PATCH 95)

configure_file( "${rocs_SOURCE_DIR}/rocsversion.h.cmake" "${rocs_BINARY_DIR}/rocsversion.h" @ONLY )

add_subdirectory(InterfacePlugins)

include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${KROSS_INCLUDES}
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
    Actions/
    GraphicsItem/
    Ui/
    LoadSave/
    Plugins/
    Scripts/
)

set( rocs_actions_SRCS
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
    Ui/LoadedPluginsDialog.cpp
    Ui/PossibleIncludes.cpp
    Ui/ScriptOutputWidget.cpp
    InterfacePlugins/ApiDoc/ApiDocWidget.cpp
    InterfacePlugins/ApiDoc/ObjectDocumentation.cpp
    InterfacePlugins/ApiDoc/PropertyDocumentation.cpp
    InterfacePlugins/ApiDoc/MethodDocumentation.cpp
    InterfacePlugins/ApiDoc/ParameterDocumentation.cpp
    InterfacePlugins/ApiDoc/ApiDocManager.cpp
    InterfacePlugins/ApiDoc/ApiDocModel.cpp
)

set( rocs_SRCS
    Main.cpp
    ${rocs_interface_SRCS}
    ${rocs_script_SRC}
)

kde4_add_ui_files(
    rocs_SRCS
    Ui/DocumentationWidget.ui
    Ui/JournalEditorWidget.ui
    Ui/DocumentTypesWidget.ui
    Ui/ConfigureDefaultProperties.ui
    Ui/ScriptOutputWidget.ui
    InterfacePlugins/ApiDoc/ApiDocWidget.ui
)

# we currently only support QtScript
set( QT_USE_QTSCRIPT true )

set( rocs_KCFGS settings.kcfgc )

kde4_add_kcfg_files( rocs_Settings_SRCS ${rocs_KCFGS} )

# needed to get icon on windows
kde4_add_app_icon( rocs_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphics/global/hi*-apps-rocs.png
)

kde4_add_executable( rocs
    ${rocs_SRCS}
    ${rocs_Settings_SRCS}
    ${rocs_RCC_SRCS}
)

target_link_libraries(rocs
    rocscore
    rocsvisualeditor
    ${KDE4_KDEUI_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KROSSUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${QT_QTXMLPATTERNS_LIBRARY}
    ${KDE4_KNEWSTUFF3_LIBS}
    ${Grantlee_CORE_LIBRARIES}
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
