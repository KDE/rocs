include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (AssignValuesToolsPlugin_SRC assignvaluestoolsplugin.cpp assignvalueswidget.cpp)

kde4_add_ui_files( AssignValuesToolsPlugin_SRC assignvalueswidget.ui )


kde4_add_plugin(rocs_assignvaluesplugin ${AssignValuesToolsPlugin_SRC} )
target_link_libraries( rocs_assignvaluesplugin
    ${KDE4_KDEUI_LIBS}
    rocslib)
                            
install( TARGETS rocs_assignvaluesplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_assignvaluesplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

