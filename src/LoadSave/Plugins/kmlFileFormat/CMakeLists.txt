project(RocsKmlFileFormatPlugin)

set(kmlFileFormatPlugin_SRCS
    KmlFileFormatPlugin.cpp
    KmlHandler.cpp
)

kde4_add_plugin(rocs_kmlfileformat ${kmlFileFormatPlugin_SRCS})

target_link_libraries(rocs_kmlfileformat
    rocslib
    rocs_GraphStructure
    ${QT_QTXML_LIBRARY}
    ${KDE4_KDEUI_LIBS}
)

install(TARGETS rocs_kmlfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_kmlfileformatplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
