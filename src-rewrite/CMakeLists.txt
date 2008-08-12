find_package(KDE4 REQUIRED)
include(KDE4Defaults)

include_directories( ${KDE4_INCLUDE_DIR} 
                     ${QT_INCLUDES} 
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_BINARY_DIR}
)

# Source Files
set(Rocs_SRCS
  # Main Entry 
  Main.cpp

  # UI Related Files
  UI_MainWindow.cpp
  UI_ConfigureDialog.cpp
  UI_FileArea.cpp
  UI_GraphLayers.cpp
  UI_GraphScene.cpp
  UI_PaletteBar.cpp
  UI_PropertiesArea.cpp
  UI_ScriptingArea.cpp

  # stolen from Step (remember to kiss  Vladimir Kuznetsov
  UI_PaletteLayout.cpp
  UI_Separator.cpp
  UI_PaletteScrollArea.cpp

  # Graph Library
  Graph.cpp
)

# Resource Files
# set (Rocs_RCCS
#    resources.qrc
# )

# Since the JavaScript supports came from QtScript... 
set (QT_USE_QTSCRIPT true)

set(Rocs_KCFGS settings.kcfgc)

# kde4_add_ui_files(Rocs_SRCS mainwindow.ui )

# qt4_add_resources(Rocs_RCC_SRCS ${Rocs_RCCS})

kde4_add_kcfg_files( Rocs_SRCS ${Rocs_KCFGS} )
kde4_add_executable(Rocs ${Rocs_SRCS} ${Rocs_RCC_SRCS})
target_link_libraries(Rocs ${KDE4_KDEUI_LIBS}  ${QT_QTSCRIPT_LIBRARY})

install(TARGETS Rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES Rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )

install( FILES 
            GraphScript.js 
            Examples.js 
            Rocsui.rc 
         DESTINATION 
            ${DATA_INSTALL_DIR}/Rocs  
)

install(FILES Rocs.kcfg DESTINATION  ${KCFG_INSTALL_DIR})

