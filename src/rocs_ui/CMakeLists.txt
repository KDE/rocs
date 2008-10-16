project(librocsui)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
		    ../rocs_core 
		    ../rocs_scripting
		    ../rocs_model
		    ../rocs_graphics)

set(librocsui_SRCS
  UI_MainWindow.cpp
  UI_OpenedFilesDockWidget.cpp
  UI_GraphLayersDockWidget.cpp
  UI_GraphScene.cpp
  UI_PaletteBarDockWidget.cpp
  UI_PaletteLayout.cpp  # stolen from Step (remember to kiss  Vladimir Kuznetsov
  UI_Separator.cpp
  UI_PaletteScrollArea.cpp
  UI_GraphEditWidget.cpp

  action_AbstractAction.cpp
  action_Pointer.cpp
  action_AddNode.cpp
  action_AddEdge.cpp
  action_MoveNode.cpp
)

kde4_add_ui_files( 
  librocsui_SRCS 
  GraphLayersDockWidget.ui
  OpenedFilesDockWidget.ui
  GraphEditWidget.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui  STATIC ${librocsui_SRCS} )
