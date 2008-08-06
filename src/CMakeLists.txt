find_package(KDE4 REQUIRED)
include(KDE4Defaults)

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# Source Files
set(Rocs_SRCS 
   main.cpp 
   DiagramScene.cpp
   DiagramNode.cpp
   DiagramEdge.cpp
   GraphEngine.cpp
   Group.cpp
   mainwindow.cpp 
)

# Resource Files
set (Rocs_RCCS
    resources.qrc
)

# Since the JavaScript supports came from QtScript... 
set (QT_USE_QTSCRIPT true)


kde4_add_ui_files(Rocs_SRCS mainwindow.ui )

qt4_add_resources(Rocs_RCC_SRCS ${Rocs_RCCS})

kde4_add_executable(Rocs ${Rocs_SRCS} ${Rocs_RCC_SRCS})

target_link_libraries(Rocs ${KDE4_KDEUI_LIBS}  ${QT_QTSCRIPT_LIBRARY})

install(TARGETS Rocs  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES Rocs.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES GraphScript.js Examples.js DESTINATION ${DATA_INSTALL_DIR}/Rocs  )
