project(RocsGmlFileFormatPlugin)

set(gmlFileFormatPlugin_SRCS
    GmlFileFormatPlugin.cpp
    GmlGrammar.cpp
    GmlGraphParsingHelper.cpp
)

kde4_add_plugin(rocs_gmlfileformat ${gmlFileFormatPlugin_SRCS})

target_link_libraries(rocs_gmlfileformat
    rocslib
    rocs_GraphStructure
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS rocs_gmlfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_gmlfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
