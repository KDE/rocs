
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	../Core
	../Engine
	../Models
	../GraphicsItem
)

set(librocsactions_SRCS
	AbstractAction.cpp
	Pointer.cpp
	AddNode.cpp
	AddEdge.cpp
	MoveNode.cpp
	Select.cpp
	SmartAction.cpp
	DeleteAction.cpp
)
kde4_add_library( librocsactions	STATIC ${librocsactions_SRCS} )
