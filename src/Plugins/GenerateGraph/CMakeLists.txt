include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (GenerateGraphToolsPlugin_SRC generategraphtoolsplugin.cpp generategraphwidget.cpp)

kde4_add_ui_files( GenerateGraphToolsPlugin_SRC generategraphwidget.ui )


kde4_add_plugin(rocs_generategraphplugin ${GenerateGraphToolsPlugin_SRC} )
target_link_libraries(  rocs_generategraphplugin
                        ${KDE4_KDEUI_LIBS}
                        rocslib
)

install( TARGETS rocs_generategraphplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_generategraphplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
