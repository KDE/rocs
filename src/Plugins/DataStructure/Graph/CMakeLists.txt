include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (GraphStructure_SRC GraphStructure.cpp GraphPlugin.cpp NodeItem.cpp EdgeItem.cpp)


kde4_add_plugin(rocs_GraphStructure ${GraphStructure_SRC} )
kde4_add_library(rocs_GraphStructure  SHARED ${GraphStructure_SRC} )

target_link_libraries(rocs_GraphStructure
                ${KDE4_KDEUI_LIBS}
                ${QT_QTSCRIPT_LIBRARY}
                ${QT_QTSCRIPTTOOLS_LIBRARY}
                rocslib)

install(TARGETS rocs_GraphStructure  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES  rocs_GraphStructure.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
