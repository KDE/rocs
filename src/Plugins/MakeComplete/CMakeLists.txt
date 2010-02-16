include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (MakeCompleToolsPlugin_SRC makecompletetoolsplugin.cpp)

kde4_add_plugin(rocs_makecompleteplugin ${MakeCompleToolsPlugin_SRC} )
target_link_libraries(rocs_makecompleteplugin 
			    ${KDE4_KDEUI_LIBS}
                            rocslib)

install(TARGETS rocs_makecompleteplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_makecompleteplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})