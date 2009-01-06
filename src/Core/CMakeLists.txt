project(librocscore)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
 
set( rocscore_LIB_SRCS
  graph.cpp
  edge.cpp
  graphDocument.cpp
  graphDocumentHandler.cpp
  node.cpp
	graphGroups.cpp
  krossBackend.cpp
  qtScriptBackend.cpp
)

kde4_add_library( librocscore  STATIC ${rocscore_LIB_SRCS} )
