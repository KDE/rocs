
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core ../Actions)
 
set( rocsgraphics_LIB_SRCS
	NodeItem.cpp
	EdgeItem.cpp
	OrientedEdgeItem.cpp
)

kde4_add_library( librocsgraphics	STATIC ${rocsgraphics_LIB_SRCS} )
