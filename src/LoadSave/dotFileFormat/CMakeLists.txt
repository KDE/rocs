project(RocsDotFileFromatPlugin)

set(dotFileFromatPlugin_SRCS
    DotFileFormatPlugin.cpp
    DotGrammar.cpp
    DotGraphParsingHelper.cpp
)

kde4_add_plugin(rocs_dotfileformat ${dotFileFromatPlugin_SRCS} )

target_link_libraries(rocs_dotfileformat
    ${KDE4_KDEUI_LIBS}
    rocslib
)

install(TARGETS rocs_dotfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_dotfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)

