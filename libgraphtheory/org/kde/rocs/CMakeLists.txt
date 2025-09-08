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
        nodeitem.cpp
        nodeitem.h
        qsgarrowheadnode.cpp
        qsgarrowheadnode.h
        qsglinenode.cpp
        qsglinenode.h
    QML_FILES
        AddEdgeAction.qml
        AddNodeAction.qml
        DeleteAction.qml
        EdgePropertyItem.qml
        Line.qml
        NodePropertyItem.qml
        Scene.qml
        SelectMoveAction.qml
        SelectionRectangle.qml
)

target_include_directories(rocsgrapheditor_backing PRIVATE
    ${CMAKE_SOURCE_DIR}/libgraphtheory
    ${CMAKE_BINARY_DIR}/libgraphtheory
)

target_link_libraries(rocsgrapheditor_backing
    PUBLIC Qt6::Core Qt6::Gui Qt6::Quick
)
