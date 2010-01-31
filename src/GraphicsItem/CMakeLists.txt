
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../Core ../Actions ../Interface)
 
set( rocsgraphics_LIB_SRCS
	NodeItem.cpp
	OrientedEdgeItem.cpp
)

kde4_add_library( librocsgraphics	STATIC ${rocsgraphics_LIB_SRCS} )
