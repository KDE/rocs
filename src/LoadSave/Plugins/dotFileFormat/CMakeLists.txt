project(RocsDotFileFormatPlugin)

set(dotFileFormatPlugin_SRCS
    DotFileFormatPlugin.cpp
    DotGrammar.cpp
    DotGraphParsingHelper.cpp
)

kde4_add_plugin(rocs_dotfileformat ${dotFileFormatPlugin_SRCS})

target_link_libraries(rocs_dotfileformat
    rocslib
    rocs_GraphStructure
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS rocs_dotfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_dotfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
