project(Rocs-rewrite)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

set(Rocs_SRCS
  Main.cpp

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
  # action_AddEdge.cpp
  # action_MoveNode.cpp

  # graphicsitem_Edge.cpp
  # graphicsitem_Graph.cpp
  graphicsitem_Node.cpp
  
  model_GraphDocument.cpp
  model_GraphLayers.cpp

  libgraph_Digraph.cpp
  libgraph_DiNode.cpp
  libgraph_Edge.cpp
  libgraph_Graph.cpp
  libgraph_GraphDocument.cpp
  libgraph_MultiGraph.cpp
  libgraph_MultiNode.cpp
  libgraph_Node.cpp
  libgraph_SimpleGraph.cpp
  libgraph_SimpleNode.cpp
)

kde4_add_ui_files( 
  Rocs_SRCS 
  GraphLayersDockWidget.ui
  OpenedFilesDockWidget.ui
  GraphEditWidget.ui
)

# Resource Files
# set (Rocs_RCCS
#    resources.qrc
# )

# Since the JavaScript supports came from QtScript... 
# set (QT_USE_QTSCRIPT true)

set(Rocs_KCFGS settings.kcfgc)

# qt4_add_resources(Rocs_RCC_SRCS ${Rocs_RCCS})

kde4_add_kcfg_files( Rocs_SRCS ${Rocs_KCFGS} )
kde4_add_executable(Rocs ${Rocs_SRCS} ${Rocs_RCC_SRCS})
target_link_libraries(Rocs ${KDE4_KDEUI_LIBS}
                           ${KDE4_KIO_LIBS}
                           ${KDE4_KTEXTEDITOR_LIBS}
                           ${QT_QTSCRIPT_LIBRARY}
)

install( TARGETS Rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES Rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES GraphScript.js Examples.js Rocsui.rc DESTINATION ${DATA_INSTALL_DIR}/Rocs  )
install( FILES Rocs.kcfg DESTINATION  ${KCFG_INSTALL_DIR} )

