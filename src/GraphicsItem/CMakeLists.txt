
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core ../Actions)
 
set( rocsgraphics_LIB_SRCS
	graphicsitem_Node.cpp
	graphicsitem_Edge.cpp
	graphicsitem_OrientedEdge.cpp
)

kde4_add_library( librocsgraphics	STATIC ${rocsgraphics_LIB_SRCS} )
