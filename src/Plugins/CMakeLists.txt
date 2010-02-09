include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory(MakeComplete)

add_subdirectory(importKML)

set(toolspluginmanager_SRC FilePluginInterface.cpp PluginManager.cpp ToolsPluginInterface.cpp)

kde4_add_library(ToolsPluginManager STATIC ${toolspluginmanager_SRC})

# target_link_libraries(ToolsPluginManager 
# 			${KDE4_KDEUI_LIBS}
# 		      )
install( FILES RocsToolsPlugin.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})
install( FILES RocsFilePlugin.desktop  DESTINATION ${SERVICETYPES_INSTALL_DIR})