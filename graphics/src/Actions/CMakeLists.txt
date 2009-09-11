
include_directories( 
	${KDE4_INCLUDES} 
	${QT_INCLUDES} 
	../Core
	../Engine
	../Models
	../GraphicsItem
	../Interface
)

set(librocsactions_SRCS
	AbstractAction.cpp
	AddNode.cpp
	AddEdge.cpp
	MoveNode.cpp
	Select.cpp
	DeleteAction.cpp
	AlignAction.cpp
)
kde4_add_library( librocsactions	STATIC ${librocsactions_SRCS} )
