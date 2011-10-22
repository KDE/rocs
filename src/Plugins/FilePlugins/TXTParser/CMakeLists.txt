
Set (PlainTxtFilePlugin_SRC PlainTxtFilePlugin.cpp)

kde4_add_plugin(rocs_plaintxt ${PlainTxtFilePlugin_SRC} )
target_link_libraries(rocs_plaintxt
                      ${KDE4_KDEUI_LIBS}
                      rocslib)

install(TARGETS rocs_plaintxt  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES rocs_plaintxtplugin.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
