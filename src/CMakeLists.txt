
include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${KROSS_INCLUDES}
    Core/
    Actions/
    GraphicsItem/
    Interface/
    Models/
    Plugins/
    Scripts/
)


# Prepare library.


set( rocs_core_SRCS
    Core/Data.cpp
    Core/DataStructure.cpp
    Core/Document.cpp
    Core/Group.cpp
    Core/DataStructurePluginInterface.cpp
    Core/DataStructurePluginManager.cpp
    Core/DocumentManager.cpp
    Core/DynamicPropertiesList.cpp
    Core/KrossBackend.cpp
    Core/Pointer.cpp
    Core/QtScriptBackend.cpp

    Plugins/PluginManager.cpp
    Plugins/ToolsPluginInterface.cpp
    Plugins/FilePluginInterface.cpp

    GraphicsItem/GraphicsLayout.cpp
    GraphicsItem/DataItem.cpp
    GraphicsItem/PointerItem.cpp
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

  Plugins/FilePluginInterface.h
  Plugins/PluginManager.h
  Plugins/ToolsPluginInterface.h

  GraphicsItem/DataItem.h
  GraphicsItem/PointerItem.h
  GraphicsItem/GraphicsLayout.h
)

set_target_properties( rocslib  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS rocslib  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES ${rocslib_LIB_HDRS}  DESTINATION ${INCLUDE_INSTALL_DIR}/rocs  COMPONENT Devel )
###################### Outros SRCS ############################
#set ( kwelcomewidget_SRCS
# KWelcomeWidget/kwelcomewidget.cpp
#)

set ( rocs_actions_SRCS
  Actions/AbstractAction.cpp
  Actions/AddConnection.cpp
  Actions/AddData.cpp
  Actions/AlignAction.cpp
  Actions/DeleteAction.cpp
  Actions/MoveData.cpp
  Actions/Select.cpp
  Actions/zoom.cpp
)

set( rocs_interface_SRCS
    Interface/ImporterExporterManager.cpp
    Interface/IncludeManagerSettings.cpp
    Interface/MainWindow.cpp
    Interface/OpenedFilesWidget.cpp
    Interface/GraphScene.cpp
    Interface/Separator.cpp
    Interface/GraphVisualEditor.cpp
    Interface/GraphPropertiesWidget.cpp
    Interface/GraphLayers.cpp
    Interface/TabWidget.cpp
    Interface/CodeEditor.cpp
    Interface/NodePropertiesWidget.cpp
    Interface/edgepropertieswidget.cpp
    Interface/ConfigureDefaultProperties.cpp
)

set( rocs_models_SRCS
  Models/model_GraphDocument.cpp
  Models/model_GraphLayers.cpp
  Models/model_GraphProperties.cpp
)

# set( rocs_undoredo_SRCS
#   UndoRedo/URAddNode.cpp
#   UndoRedo/URAddEdge.cpp
# )

set ( rocs_script_SRC
        Interface/PossibleIncludes.cpp
        Scripts/IncludeManager.cpp
)

set(rocs_SRCS
  Main.cpp
  ${rocs_actions_SRCS}
  ${rocs_interface_SRCS}
  ${rocs_models_SRCS}
  ${rocs_script_SRC}
)

kde4_add_ui_files(
	rocs_SRCS
	Interface/OpenedFilesWidget.ui
	Interface/GraphPropertiesWidget.ui
	Interface/NodePropertiesWidget.ui
	Interface/EdgePropertiesWidget.ui
	Interface/ConfigureDefaultProperties.ui
)

set (QT_USE_QTSCRIPT true)

set(rocs_KCFGS settings.kcfgc)

kde4_add_kcfg_files(rocs_Settings_SRCS ${rocs_KCFGS} )
kde4_add_executable(rocs ${rocs_SRCS} ${rocs_Settings_SRCS} ${rocs_RCC_SRCS})

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
install( FILES rocs.kcfg DESTINATION  ${KCFG_INSTALL_DIR} )

install(FILES rocs.knsrc DESTINATION  ${CONFIG_INSTALL_DIR})

install( FILES
      Core/RocsDataStructurePlugin.desktop
	  Plugins/RocsToolsPlugin.desktop
	  Plugins/RocsFilePlugin.desktop
	DESTINATION
	    ${SERVICETYPES_INSTALL_DIR}
)



add_subdirectory(Plugins)

add_subdirectory(Scripts)

#Build Tests if KDE_TEST is on
if(KDE4_BUILD_TESTS)
#     enable_testing()
#     add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)

