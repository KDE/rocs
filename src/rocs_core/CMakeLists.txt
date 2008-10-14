project(librocscore)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
 
set( rocscore_LIB_SRCS
  libgraph_Digraph.cpp       libgraph_DiNode.cpp        libgraph_Edge.cpp          
  libgraph_Graph.cpp         libgraph_GraphDocument.cpp libgraph_MultiGraph.cpp  
  libgraph_MultiNode.cpp     libgraph_Node.cpp          libgraph_SimpleGraph.cpp   
  libgraph_SimpleNode.cpp
)

kde4_add_library( librocscore  STATIC ${rocscore_LIB_SRCS} )
