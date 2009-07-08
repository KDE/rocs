
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	../Core
	../Engine
	../Models
	../GraphicsItem
	../Actions
)

set(librocsui_SRCS
	SUI_MainWindow.cpp
	SUI_OpenedFilesWidget.cpp
	SUI_GraphToolBoxWidget.cpp
	SUI_GraphScene.cpp
	SUI_PaletteBarWidget.cpp
	SUI_PaletteLayout.cpp	# stolen from Step (remember to kiss	Vladimir Kuznetsov
	SUI_Separator.cpp
	SUI_PaletteScrollArea.cpp
	SUI_GraphVisualEditor.cpp
	SUI_GraphPropertiesWidget.cpp

	TabWidget.cpp
)

kde4_add_ui_files( 
	librocsui_SRCS 
	GraphToolBox.ui
	OpenedFilesWidget.ui
	GraphEditWidget.ui
	GraphPropertiesWidget.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui	STATIC ${librocsui_SRCS} )
