project(librocscore)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
 
set( rocscore_LIB_SRCS
  Graph.cpp
  Edge.cpp
  GraphDocument.cpp
  Node.cpp
  OrientedGraph.cpp
  OrientedNode.cpp
  MultiGraph.cpp
  MultiNode.cpp
)

kde4_add_library( librocscore  STATIC ${rocscore_LIB_SRCS} )
