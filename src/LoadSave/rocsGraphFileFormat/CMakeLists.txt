project(RocsGraphFileFormatPlugin)

set(rocsGraphFileFromatPlugin_SRCS
    RocsGraphFileFormatPlugin.cpp
)

kde4_add_plugin(rocs_rocsgraphfileformat ${rocsGraphFileFromatPlugin_SRCS})

target_link_libraries(rocs_rocsgraphfileformat
    ${KDE4_KDEUI_LIBS}
    rocslib
)

install(TARGETS rocs_rocsgraphfileformat DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES rocs_rocsgraphfileformat.desktop DESTINATION ${SERVICES_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif(KDE4_BUILD_TESTS)
