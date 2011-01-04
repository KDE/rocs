
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
  AddConnection.cpp
  AddData.cpp
  AlignAction.cpp
  DeleteAction.cpp
  MoveData.cpp
  Select.cpp

)
kde4_add_library( librocsactions	STATIC ${librocsactions_SRCS} )
