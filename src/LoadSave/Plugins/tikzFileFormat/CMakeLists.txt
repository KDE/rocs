project(RocsTikzFileFormatPlugin)

set(tikzFileFormatPlugin_SRCS
    TikzFileFormatPlugin.cpp
)

kde4_add_plugin(rocs_tikzfileformat ${tikzFileFormatPlugin_SRCS} )

target_link_libraries(rocs_tikzfileformat
    ${KDE4_KDEUI_LIBS}
    rocslib
)

install(TARGETS rocs_tikzfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_tikzfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)