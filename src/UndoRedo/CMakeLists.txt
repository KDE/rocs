include_directories( 
  ${KDE4_INCLUDES} 
  ${QT_INCLUDES} 
  ../Core 
  ../Actions 
  ../Interface
)
 
set( rocsundoredo_LIB_SRCS
  URAddNode.cpp
  URAddEdge.cpp
)

kde4_add_library( librocsundoredo STATIC ${rocsundoredo_LIB_SRCS} )
 
