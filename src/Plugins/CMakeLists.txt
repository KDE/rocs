include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory(MakeComplete)

set(toolspluginmanager_SRC toolspluginmanager.cpp toolspluginmanager.h)

kde4_add_library(ToolsPluginManager STATIC ${toolspluginmanager_SRC})

# target_link_libraries(ToolsPluginManager 
# 			${KDE4_KDEUI_LIBS}
# 		      )