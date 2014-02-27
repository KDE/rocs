##
#   This file is part of Rocs.
#   Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>
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

include_directories(
    ${ROCSCORE_INCLUDES}
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Prepare library.
set( rocsvisualeditor_scene_SRCS
    Scene/DataItem.cpp
    Scene/GraphicsLayout.cpp
    Scene/GraphScene.cpp
    Scene/PointerItem.cpp
)

set( rocsvisualeditor_actions_SRCS
    Actions/AlignAction.cpp
    Actions/AbstractAction.cpp
    Actions/AddConnectionHandAction.cpp
    Actions/AddDataAction.cpp
    Actions/DeleteAction.cpp
    Actions/ZoomAction.cpp
    Actions/AddDataHandAction.cpp
    Actions/DeleteHandAction.cpp
    Actions/AssignValueAction.cpp
    Actions/SelectMoveHandAction.cpp
    Actions/PropertiesDialogAction.cpp
)

set( rocsvisualeditor_interface_SRCS
    Interface/DataPropertiesWidget.cpp
    Interface/DataStructurePropertiesDialog.cpp
    Interface/DocumentPropertiesDialog.cpp
    Interface/DataStructurePage.cpp
    Interface/DataTypePage.cpp
    Interface/PointerTypePage.cpp
    Interface/PointerPropertiesWidget.cpp
    Interface/EditorToolbar.cpp

    Interface/model_GraphProperties.cpp
)

set( rocsvisualeditor_tools_SRCS
    Tools/ToolManager.cpp
    Tools/ToolsPluginInterface.cpp
)

set( rocsvisualeditor_SRCS
    GraphVisualEditor.cpp

    ${rocsvisualeditor_actions_SRCS}
    ${rocsvisualeditor_scene_SRCS}
    ${rocsvisualeditor_interface_SRCS}
    ${rocsvisualeditor_tools_SRCS}
)

kde4_add_ui_files( rocsvisualeditor_SRCS
    Interface/DataStructurePage.ui
    Interface/DataTypePage.ui
    Interface/PointerTypePage.ui
    Interface/DataPropertiesWidget.ui
    Interface/PointerPropertiesWidget.ui
)

kde4_add_library( rocsvisualeditor SHARED ${rocsvisualeditor_SRCS} )

target_link_libraries(rocsvisualeditor
    rocscore
    KF5::Service
)

set( rocsvisualeditor_LIB_HDRS
    Scene/DataItem.h
    Scene/PointerItem.h
    GraphVisualEditor.h
)

add_subdirectory(Tools)

set_target_properties( rocsvisualeditor PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS rocsvisualeditor  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES ${rocsvisualeditor_LIB_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/rocs  COMPONENT Devel )

################## INSTALLS ##########################

install( FILES
    Tools/RocsToolsPlugin.desktop
    DESTINATION
    ${SERVICETYPES_INSTALL_DIR}
)

# Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory( Tests )
endif(KDE4_BUILD_TESTS)
