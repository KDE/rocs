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
set(ROCS_VERSION_MINOR 7)
set(ROCS_VERSION_PATCH 70)

configure_file( "${rocs_SOURCE_DIR}/rocsversion.h.cmake" "${rocs_BINARY_DIR}/rocsversion.h" @ONLY )

#Sice building some load/save plugins can take a long time, you can disable it.
option(ROCS_BUILD_LOADSAVE "ROCS_BUILD_LOADSAVE" ON)

include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${KROSS_INCLUDES}
    Core/
    Actions/
    GraphicsItem/
    Interface/
    Interface/VisualEditor
    LoadSave/
    Models/
    Plugins/
    Scripts/
)


# Prepare library.
set( rocs_core_SRCS
    Core/DataType.cpp
    Core/PointerType.cpp
    Core/Data.cpp
    Core/DataStructure.cpp
    Core/Document.cpp
    Core/Group.cpp
    Core/Project.cpp
    Core/DataStructurePluginInterface.cpp
    Core/DataStructurePluginManager.cpp
    Core/DocumentManager.cpp
    Core/DynamicPropertiesList.cpp
    Core/KrossBackend.cpp
    Core/Pointer.cpp
    Core/QtScriptBackend.cpp
    Core/Modifiers/Topology.cpp
    Core/Modifiers/ValueModifier.cpp

    Plugins/PluginManager.cpp
    Plugins/ToolsPluginInterface.cpp

    LoadSave/GraphFileBackendManager.cpp
    LoadSave/GraphFilePluginInterface.cpp
    LoadSave/RocsGraphFileFormatPlugin.cpp

    GraphicsItem/GraphicsLayout.cpp
    GraphicsItem/DataItem.cpp
    GraphicsItem/PointerItem.cpp

    Scripts/IncludeManager.cpp
)

kde4_add_library( rocslib  SHARED ${rocs_core_SRCS} )

target_link_libraries( rocslib
    ${KDE4_KROSSCORE_LIBS}
    ${KDE4_KIO_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
)

set( rocslib_LIB_HDRS
    Core/DynamicPropertiesList.h
    Core/Data.h
    Core/DataStructure.h
    Core/Document.h
    Core/KrossBackend.h
    Core/Pointer.h
    Core/QtScriptBackend.h
    Core/rocslib_export.h
    Core/Rocs_Typedefs.h
    Core/DataStructurePluginInterface.h
    Core/DataStructurePluginManager.h

    Core/Modifiers/Topology.h

    LoadSave/GraphFilePluginInterface.h
    Plugins/PluginManager.h
    Plugins/ToolsPluginInterface.h

    GraphicsItem/DataItem.h
    GraphicsItem/PointerItem.h
    GraphicsItem/GraphicsLayout.h

    Scripts/IncludeManager.h
)

set_target_properties( rocslib  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS rocslib  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES ${rocslib_LIB_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/rocs  COMPONENT Devel )


set( rocs_actions_SRCS
    Actions/AbstractAction.cpp
    Actions/AddConnectionHandAction.cpp
    Actions/AddDataAction.cpp
    Actions/AlignAction.cpp
    Actions/DeleteAction.cpp
    Actions/ZoomAction.cpp
    Actions/AddDataHandAction.cpp
    Actions/DeleteHandAction.cpp
    Actions/AssignValueAction.cpp
    Actions/SelectMoveHandAction.cpp
    Actions/PropertiesDialogAction.cpp
)

set( rocs_interface_SRCS
    Interface/ImporterExporterManager.cpp
    Interface/IncludeManagerSettings.cpp
    Interface/MainWindow.cpp
    Interface/OpenedFilesWidget.cpp
    Interface/Separator.cpp
    Interface/TabWidget.cpp
    Interface/CodeEditor.cpp
    Interface/ConfigureDefaultProperties.cpp
    Interface/DataPropertiesWidget.cpp
    Interface/DocumentTypesWidget.cpp
    Interface/DataStructurePropertiesDialog.cpp
    Interface/DocumentPropertiesDialog.cpp
    Interface/DataStructurePage.cpp
    Interface/DataTypePage.cpp
    Interface/LoadedPluginsDialog.cpp
    Interface/PointerTypePage.cpp
    Interface/PointerPropertiesWidget.cpp
    Interface/VisualEditor/GraphScene.cpp
    Interface/VisualEditor/GraphVisualEditor.cpp
)

set( rocs_models_SRCS
    Models/model_GraphDocument.cpp
    Models/model_GraphLayers.cpp
    Models/model_GraphProperties.cpp
)

set( rocs_script_SRC
    Interface/PossibleIncludes.cpp
)

set( rocs_SRCS
    Main.cpp
    ${rocs_actions_SRCS}
    ${rocs_interface_SRCS}
    ${rocs_models_SRCS}
    ${rocs_script_SRC}
)

kde4_add_ui_files(
    rocs_SRCS
    Interface/OpenedFilesWidget.ui
    Interface/DocumentTypesWidget.ui
    Interface/DataStructurePage.ui
    Interface/DataTypePage.ui
    Interface/PointerTypePage.ui
    Interface/DataPropertiesWidget.ui
    Interface/PointerPropertiesWidget.ui
    Interface/ConfigureDefaultProperties.ui
)

# we currently only support QtScript
set( QT_USE_QTSCRIPT true )

set( rocs_KCFGS settings.kcfgc )

kde4_add_kcfg_files( rocs_Settings_SRCS ${rocs_KCFGS} )

kde4_add_executable( rocs
    ${rocs_SRCS}
    ${rocs_Settings_SRCS}
    ${rocs_RCC_SRCS}
)

target_link_libraries(rocs
    rocslib
    ${KDE4_KDEUI_LIBS}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KROSSUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTSCRIPTTOOLS_LIBRARY}
    ${KDE4_KNEWSTUFF3_LIBS}
)

################## INSTALLS ##########################

install( TARGETS rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( PROGRAMS rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs  )
install( FILES rocs.kcfg DESTINATION ${KCFG_INSTALL_DIR} )
install( FILES rocs.knsrc DESTINATION ${CONFIG_INSTALL_DIR} )

install( FILES
    Core/RocsDataStructurePlugin.desktop
    Plugins/RocsToolsPlugin.desktop
    LoadSave/RocsGraphFilePlugin.desktop
    DESTINATION
    ${SERVICETYPES_INSTALL_DIR}
)

add_subdirectory( Plugins )

#### Build Load/Save plugins.
if (ROCS_BUILD_LOADSAVE)
    add_subdirectory( LoadSave )
else (ROCS_BUILD_LOADSAVE)
    message("-----------------------------------------------------------------")
    message("                          ATTENTION                              ")
    message("     The build and install of Load/Save plugins are DISABLED     ")
    message("    To reactivate it, set the option ROCS_BUILD_LOADSAVE to on   ")
    message("-----------------------------------------------------------------")
endif(ROCS_BUILD_LOADSAVE)

add_subdirectory( Scripts )

# Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory( Tests )
endif(KDE4_BUILD_TESTS)

