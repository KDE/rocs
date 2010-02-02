include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (MakeCompleToolsPlugin_SRC makecompletetoolsplugin.cpp)

kde4_add_plugin(MakeCompleteToolsPlugin ${MakeCompleToolsPlugin_SRC}   )
target_link_libraries(MakeCompleteToolsPlugin 
			    ${KDE4_KDEUI_LIBS})
