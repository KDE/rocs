include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (TransformEdgesToolsPlugin_SRC transformedgestoolsplugin.cpp transformedgeswidget.cpp)

kde4_add_ui_files( TransformEdgesToolsPlugin_SRC transformedgeswidget.ui )

kde4_add_plugin(rocs_transformedgesplugin ${TransformEdgesToolsPlugin_SRC} )
target_link_libraries( rocs_transformedgesplugin
                       rocs_GraphStructure
                       ${KDE4_KDEUI_LIBS}
                       rocslib)
                            
install( TARGETS rocs_transformedgesplugin  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_transformedgesplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

