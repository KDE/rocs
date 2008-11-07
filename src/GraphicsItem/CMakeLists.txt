project(librocsgraphics)
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core)
 
set( rocsgraphics_LIB_SRCS
  graphicsitem_Node.cpp
  graphicsitem_Edge.cpp
  graphicsitem_MultiEdge.cpp
  graphicsitem_OrientedEdge.cpp
)

kde4_add_library( librocsgraphics  STATIC ${rocsgraphics_LIB_SRCS} )
