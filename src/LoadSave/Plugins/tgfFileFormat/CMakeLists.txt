project(RocsTgfFileFormatPlugin)

set(tgfFileFromatPlugin_SRCS
    TgfFileFormatPlugin.cpp
)

kde4_add_plugin(rocs_tgffileformat ${tgfFileFromatPlugin_SRCS} )

target_link_libraries(rocs_tgffileformat
    ${KDE4_KDEUI_LIBS}
    rocslib
)

install(TARGETS rocs_tgffileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_tgffileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
