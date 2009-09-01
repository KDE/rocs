
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	../Core
	../Engine
	../Models
	../GraphicsItem
	../Actions
)

set(librocsui_SRCS
	MainWindow.cpp
	OpenedFilesWidget.cpp
	GraphScene.cpp
	Separator.cpp
	GraphVisualEditor.cpp
	GraphPropertiesWidget.cpp
	GraphLayers.cpp
	TabWidget.cpp
	CodeEditor.cpp
)

kde4_add_ui_files( 
	librocsui_SRCS 
	OpenedFilesWidget.ui
	GraphEditWidget.ui
	GraphPropertiesWidget.ui
	AddMultiNodes.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui	STATIC ${librocsui_SRCS} )
