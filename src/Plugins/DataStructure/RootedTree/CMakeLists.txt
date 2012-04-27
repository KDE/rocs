



#project(rocs_rootedtree_structure)

#find_package(KDE4 REQUIRED)
#include(KDE4Defaults)
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(rocs_RootedTree_SRCS RootedTreeNode.cpp RootedTreeStructure.cpp RootedTreePlugin.cpp RootedTreeNodeItem.cpp RootedTreeEdgeItem.cpp)

kde4_add_plugin(rocs_RootedTreeStructure ${rocs_RootedTree_SRCS})

target_link_libraries(rocs_RootedTreeStructure 
                ${KDE4_KDEUI_LIBS}
                ${QT_QTSCRIPT_LIBRARY}
                ${QT_QTSCRIPTTOOLS_LIBRARY}
                rocslib)

install(TARGETS rocs_RootedTreeStructure DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES RootedTreeui.rc DESTINATION ${DATA_INSTALL_DIR}/rocs_rootedtree)

install(FILES rocs_RootedTreeStructure.desktop DESTINATION ${SERVICES_INSTALL_DIR})

