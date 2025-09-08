# SPDX-License-Identifier: BSD-3-Clause
# SPDX-FileCopyrightText: Andreas Cord-Landwehr <cordlandwehr@kde.org>

add_library(rocsgrapheditor_backing STATIC)

qt6_add_qml_module(rocsgrapheditor_backing
    URI org.kde.rocs
    DEPENDENCIES
        QtQuick
    SOURCES
        edgeitem.cpp
        edgeitem.h
        edgemodel.cpp
        edgemodel.h
        edgeproxy.cpp
        edgeproxy.h
        edgepropertymodel.cpp
        edgepropertymodel.h
        edgetypemodel.cpp
        edgetypemodel.h
        edgetypepropertymodel.cpp
        edgetypepropertymodel.h
        graph.cpp
        graph.h
        nodeproxy.cpp
        nodeproxy.h
        nodeitem.cpp
        nodeitem.h
        nodemodel.cpp
        nodemodel.h
        nodepropertymodel.cpp
        nodepropertymodel.h
        nodetypemodel.cpp
        nodetypemodel.h
        nodetypepropertymodel.cpp
        nodetypepropertymodel.h
        qsgarrowheadnode.cpp
        qsgarrowheadnode.h
        qsglinenode.cpp
        qsglinenode.h
    QML_FILES
        EdgePropertyItem.qml
        Line.qml
        NodePropertyItem.qml
        Scene.qml
        GraphSelectionRectangle.qml
)

target_include_directories(rocsgrapheditor_backing PRIVATE
    ${CMAKE_SOURCE_DIR}/libgraphtheory
    ${CMAKE_BINARY_DIR}/libgraphtheory
)

target_link_libraries(rocsgrapheditor_backing
    PUBLIC Qt6::Core Qt6::Gui Qt6::Quick
)
