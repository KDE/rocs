
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	../Core
	../Engine
	../Models
	../GraphicsItem
	../Actions
	../Plugins
)

set(librocsui_SRCS edgepropertieswidget.cpp
	MainWindow.cpp
	OpenedFilesWidget.cpp
	GraphScene.cpp
	Separator.cpp
	GraphVisualEditor.cpp
	GraphPropertiesWidget.cpp
	GraphLayers.cpp
	TabWidget.cpp
	CodeEditor.cpp
	NodePropertiesWidget.cpp
)

kde4_add_ui_files( 
	librocsui_SRCS 
	OpenedFilesWidget.ui
	GraphPropertiesWidget.ui
	NodePropertiesWidget.ui
	EdgePropertiesWidget.ui
	AddMultiNodes.ui
)

set(librocsui_KCFGS ../settings.kcfgc)
kde4_add_kcfg_files( librocsui_SRCS ${librocsui_KCFGS} )
kde4_add_library( librocsui STATIC ${librocsui_SRCS} )
target_link_libraries( librocsui
	      ToolsPluginManager
)
