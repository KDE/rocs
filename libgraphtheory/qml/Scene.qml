/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0
import org.kde.rocs.graphtheory 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 600

    signal createNode(real x, real y);
    signal createEdge(Node from, Node to);
    signal deleteNode(Node node);
    signal deleteEdge(Edge edge);

    property Node __createEdgeFromBuffer

    Rectangle { // white background
        color: "white"
        anchors.fill: parent
    }

    ExclusiveGroup {
        id: editToolButton
    }

    ColumnLayout {
        id: toolbar
        ToolButton {
            id: buttonSelectMove
            iconName: "rocsselectmove"
            checkable: true
            checked: true
            exclusiveGroup: editToolButton
        }
        ToolButton {
            id: buttonAddNode
            iconName: "rocsaddnode"
            checkable: true
            exclusiveGroup: editToolButton
        }
        ToolButton {
            id: buttonAddEdge
            iconName: "rocsaddedge"
            checkable: true
            exclusiveGroup: editToolButton
        }
        ToolButton {
            id: buttonDelete
            iconName: "rocsdelete"
            checkable: true
            exclusiveGroup: editToolButton
        }
    }

    Item {
        id: scene
        width: root.width - toolbar.width - 20
        height: root.height
        anchors {
            left: toolbar.right
            leftMargin: 10
        }

        MouseArea {
            id: sceneAction
            anchors.fill: parent
            property int startX
            property int startY
            property int endX
            property int endY
            property bool activePress
            onClicked: {
                console.log("scene: CLICKED")
                if (buttonAddNode.checked) {
                    mouse.accepted = true
                    createNode(mouse.x, mouse.y);
                    return
                }
            }
            onPressed: {
                activePress = true
                startX = mouse.x
                startY = mouse.y
                endX = mouse.x
                endY = mouse.y
            }
            onPositionChanged: {
                endX = mouse.x
                endY = mouse.y
                console.log("scene: RELEASED" + mouse.x)
            }
            onReleased: {
                activePress = false
            }
        }

        // highlighter for selection and edges
        Component {
            id: rectComponent
            Rectangle {
                x: Math.min(sceneAction.startX, sceneAction.endX)
                y: Math.min(sceneAction.startY, sceneAction.endY)
                width: Math.abs(sceneAction.startX - sceneAction.endX)
                height: Math.abs(sceneAction.startY - sceneAction.endY)
                color: "#afc3deff"
                border.color: "steelblue"
                border.width: 2
            }
        }
        Component {
            id: lineComponent
            Line {
                fromX: sceneAction.startX
                fromY: sceneAction.startY
                toX: sceneAction.endX
                toY: sceneAction.endY
            }
        }
        Loader {
            sourceComponent: {
                if (buttonSelectMove.checked && sceneAction.activePress) {
                    return rectComponent
                }
                if (buttonAddEdge.checked && sceneAction.activePress) {
                    return lineComponent
                }
                return undefined
            }
        }

        Repeater {
            model: edgeModel
            EdgeItem {
                edge: model.dataRole

                EdgePropertyItem {
                    anchors.centerIn: parent
                    edge: model.dataRole
                }

                MouseArea {
                    anchors.fill: parent
                    propagateComposedEvents: true
                    onReleased: {
                        if (buttonDelete.checked) {
                            mouse.accepted = true
                            deleteEdge(edge)
                        }
                        mouse.accepted = false
                    }
                }
            }
        }

        Repeater {
            model: nodeModel
            NodeItem {
                node: model.dataRole

                NodePropertyItem {
                    anchors.centerIn: parent
                    node: model.dataRole
                }

                Drag.active: dragArea.drag.active
                MouseArea {
                    id: dragArea
                    anchors.fill: parent
                    propagateComposedEvents: true
                    drag.target: { // only enable drag when move/select checked
                        buttonSelectMove.checked ? parent : undefined
                    }
                    onClicked: {
                        if (buttonDelete.checked) {
                            mouse.accepted = true
                            deleteNode(node)
                            return
                        }
                        mouse.accepted = false
                    }
                }
            }
        }
    }
}
