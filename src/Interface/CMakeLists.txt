project(librocsui)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
		    ../Core
		    ../Engine
		    ../Models
		    ../GraphicsItem)

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
  UI_PropertiesArea.cpp

  action_AbstractAction.cpp
  action_Pointer.cpp
  action_AddNode.cpp
  action_AddEdge.cpp
  action_MoveNode.cpp
  action_SingleSelect.cpp
)

kde4_add_ui_files( 
  librocsui_SRCS 
  GraphLayersDockWidget.ui
  OpenedFilesDockWidget.ui
  GraphEditWidget.ui
  GraphPropertiesWidget.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui  STATIC ${librocsui_SRCS} )
