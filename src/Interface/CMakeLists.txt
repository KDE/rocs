
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
	GraphToolBoxWidget.cpp
	GraphScene.cpp
	PaletteBarWidget.cpp
	PaletteLayout.cpp	# stolen from Step (remember to kiss	Vladimir Kuznetsov
	Separator.cpp
	PaletteScrollArea.cpp
	GraphVisualEditor.cpp
	GraphPropertiesWidget.cpp

	TabWidget.cpp
	CodeEditor.cpp
)

kde4_add_ui_files( 
	librocsui_SRCS 
	GraphToolBox.ui
	OpenedFilesWidget.ui
	GraphEditWidget.ui
	GraphPropertiesWidget.ui
	AddMultiNodes.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui	STATIC ${librocsui_SRCS} )
