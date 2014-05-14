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
            action: SelectMoveAction {
                id: selectMoveAction
                exclusiveGroup: editToolButton
                checked: true
            }
        }
        ToolButton {
            action: AddNodeAction {
                id: addNodeAction
                exclusiveGroup: editToolButton
            }
        }
        ToolButton {
            action: AddEdgeAction {
                id: addEdgeAction
                exclusiveGroup: editToolButton
                onCreateEdge: root.createEdge(from, to)
            }
        }
        ToolButton {
            action: DeleteAction {
                id: deleteAction
                exclusiveGroup: editToolButton
            }
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
            property int mouseX
            property int mouseY

            signal deleteHighlighted();

            focus: true

            onClicked: {
                if (addNodeAction.checked) {
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
                mouseX = mouse.x
                mouseY = mouse.y
                endX = mouse.x
                endY = mouse.y
            }
            onReleased: {
                activePress = false
                if (addEdgeAction.checked) {
                    addEdgeAction.apply();
                }
            }

            Keys.onPressed: {
                if (event.key == Qt.Key_Delete) {
                    deleteHighlighted();
                    event.accepted = true;
                }
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
                if (selectMoveAction.checked && sceneAction.activePress) {
                    return rectComponent
                }
                if (addEdgeAction.checked && sceneAction.activePress) {
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
                    onClicked: {
                        if (deleteAction.checked) {
                            deleteEdge(edge)
                        } else {
                            mouse.accepted = false
                        }
                    }
                    onPressed: {
                        if (deleteAction.checked) {
                            mouse.accepted = true
                        } else {
                            mouse.accepted = false
                        }
                    }
                }
            }
        }

        Repeater {
            model: nodeModel
            NodeItem {
                id: nodeItem
                node: model.dataRole
                highlighted: addEdgeAction.from == node || addEdgeAction.to == node
                Connections {
                    target: sceneAction
                    onMouseXChanged: {
                        if (nodeItem.contains(Qt.point(sceneAction.mouseX,sceneAction.mouseY))) {
                            if (addEdgeAction.from == null) {
                                addEdgeAction.from = node;
                            } else {
                                addEdgeAction.to = node
                            }
                        }
                    }
                    onMouseYChanged: {
                        if (nodeItem.contains(Qt.point(sceneAction.mouseX,sceneAction.mouseY))) {
                            if (addEdgeAction.from == null) {
                                addEdgeAction.from = node;
                            } else {
                                addEdgeAction.to = node
                            }
                        }
                    }
                    onDeleteHighlighted: {
                        if (highlighted) {
                            deleteNode(node)
                        }
                    }
                }
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
                        selectMoveAction.checked ? parent : undefined
                    }
                    onClicked: {
                        if (deleteAction.checked) {
                            deleteNode(nodeItem.node)
                        } else {
                            mouse.accepted = false
                        }
                    }
                    onPressed: {
                        if (!(deleteAction.checked || selectMoveAction.checked)) {
                            mouse.accepted = false
                        }
                    }
                }
            }
        }
    }
}
