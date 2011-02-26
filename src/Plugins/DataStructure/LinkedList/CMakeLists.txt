include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)


Set (ListStructure_SRC 
    ListNode.cpp 
    ListStructure.cpp 
    ListPlugin.cpp 
    NodeListItem.cpp 
    LinkedListPointerItem.cpp)

kde4_add_plugin(rocs_ListStructure ${ListStructure_SRC} )
target_link_libraries(rocs_ListStructure
                ${KDE4_KDEUI_LIBS}
                ${QT_QTSCRIPT_LIBRARY}
                ${QT_QTSCRIPTTOOLS_LIBRARY}
                rocslib)

install(TARGETS rocs_ListStructure  DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES  rocs_ListStructure.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
