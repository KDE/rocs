include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

#ADD_DEFINITIONS(${QT_DEFINITIONS})
#ADD_DEFINITIONS(-DQT_PLUGIN)
#ADD_DEFINITIONS(-DQT_NO_DEBUG)
#ADD_DEFINITIONS(-DQT_SHARED)


Set (KMLFilePlugin_SRC KMLFilePlugin.cpp)

kde4_add_plugin(rocs_kmlfileplugin ${KMLFilePlugin_SRC} )
target_link_libraries(rocs_kmlfileplugin
			    ${KDE4_KDEUI_LIBS}
                            rocslib)

install(TARGETS rocs_kmlfileplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_kmlfileplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
