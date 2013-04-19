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
set(ROCS_VERSION_PATCH 0)

configure_file( "${rocs_SOURCE_DIR}/rocsversion.h.cmake" "${rocs_BINARY_DIR}/rocsversion.h" @ONLY )

include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${KROSS_INCLUDES}
    ${ROCSCORE_INCLUDES}
    ${ROCSVISUALEDITOR_INCLUDES}
    Actions/
    GraphicsItem/
    Interface/
    Interface/VisualEditor
    LoadSave/
    Models/
    Plugins/
    Scripts/
)

set( rocs_actions_SRCS
    Actions/PropertiesDialogAction.cpp
)

set( rocs_interface_SRCS
    Interface/DocumentationWidget.cpp
    Interface/JournalEditorWidget.cpp
    Interface/SideDockWidget.cpp
    Interface/ImporterExporterManager.cpp
    Interface/IncludeManagerSettings.cpp
    Interface/MainWindow.cpp
    Interface/Separator.cpp
    Interface/TabWidget.cpp
    Interface/CodeEditor.cpp
    Interface/DocumentTypesWidget.cpp
    Interface/ConfigureDefaultProperties.cpp
    Interface/LoadedPluginsDialog.cpp
    Interface/PossibleIncludes.cpp
    Interface/ScriptOutputWidget.cpp
)

set( rocs_SRCS
    Main.cpp
    ${rocs_interface_SRCS}
    ${rocs_script_SRC}
)

kde4_add_ui_files(
    rocs_SRCS
    Interface/DocumentationWidget.ui
    Interface/JournalEditorWidget.ui
    Interface/DocumentTypesWidget.ui
    Interface/ConfigureDefaultProperties.ui
    Interface/ScriptOutputWidget.ui
)

# we currently only support QtScript
set( QT_USE_QTSCRIPT true )

set( rocs_KCFGS settings.kcfgc )

kde4_add_kcfg_files( rocs_Settings_SRCS ${rocs_KCFGS} )

# needed to get icon on windows
kde4_add_app_icon( rocs_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../graphics/global/hi*-app-rocs.png
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
    ${KDE4_KNEWSTUFF3_LIBS}
)

################## INSTALLS ##########################

install( TARGETS rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( PROGRAMS rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs  )
install( FILES rocs.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES rocs.knsrc DESTINATION ${CONFIG_INSTALL_DIR} )

add_subdirectory( Scripts )

# Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)


