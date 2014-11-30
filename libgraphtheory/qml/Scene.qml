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
import QtQml.StateMachine 1.0 as DSM
import org.kde.rocs.graphtheory 1.0

Item {
    id: root
    width: 800
    height: 600

    focus: true

    // element create/remove actions
    signal createNode(real x, real y);
    signal createEdge(Node from, Node to);
    signal deleteNode(Node node);
    signal deleteEdge(Edge edge);

    // exec dialog signals
    signal showNodePropertiesDialog(Node node);
    signal showEdgePropertiesDialog(Edge edge);

    Connections {
        target: selectMoveAction
        onToggled: {
            dsmSelectMove.running = selectMoveAction.checked
        }
    }
    Connections {
        target: addEdgeAction
        onToggled: {
            dsmCreateEdge.running = addEdgeAction.checked
        }
    }

    Keys.onPressed: {
        switch(event.key) {
        case Qt.Key_Escape:
            scene.clearSelection();
            event.accepted = true;
            break;
        case Qt.Key_Delete:
            scene.deleteSelected()
            event.accepted = true;
            break;
        default:
            break;
        }
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
    ScrollView {
        id: sceneScrollView
        width: root.width - toolbar.width - 20
        height: root.height
        anchors {
            left: toolbar.right
            leftMargin: 10
        }


        Rectangle { // white background
            color: "white"
            width: parent.width
            height: parent.height
        }

        Item {
            id: scene
            width: sceneScrollView.width - 30
            height: sceneScrollView.height - 20
            property variant origin: Qt.point(0, 0) // coordinate of global origin (0,0) in scene
            signal deleteSelected();
            signal updateSelection();
            signal createEdgeUpdateFromNode();
            signal createEdgeUpdateToNode();
            function clearSelection()
            {
                selectionRect.from = Qt.point(0, 0)
                selectionRect.to = Qt.point(0, 0)
                updateSelection();
            }
            function setEdgeFromNode()
            {
                addEdgeAction.to = null
                createEdgeUpdateFromNode();
            }

            MouseArea {
                id: sceneAction
                anchors.fill: parent

                property variant lastMouseClicked: Qt.point(0, 0)
                property variant lastMousePressed: Qt.point(0, 0)
                property variant lastMouseReleased: Qt.point(0, 0)
                property variant lastMousePosition: Qt.point(0, 0)

                onClicked: {
                    lastMouseClicked = Qt.point(mouse.x, mouse.y)
                    if (addNodeAction.checked) {
                        mouse.accepted = true
                        createNode(mouse.x + scene.origin.x, mouse.y + scene.origin.y);
                        return
                    }
                }
                onPressed: {
                    lastMousePressed = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: {
                    lastMousePosition = Qt.point(mouse.x, mouse.y)
                }
                onReleased: {
                    lastMouseReleased = Qt.point(mouse.x, mouse.y)
                }
            }

            SelectionRectangle {
                id: selectionRect
                visible: false
            }

            Line {
                id: createLineMarker
                visible: false
                fromX: sceneAction.lastMousePressed.x
                fromY: sceneAction.lastMousePressed.y
                toX: sceneAction.lastMousePosition.x
                toY: sceneAction.lastMousePosition.y
            }

            Repeater {
                model: edgeModel
                EdgeItem {
                    id: edgeItem
                    edge: model.dataRole
                    origin: scene.origin

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
                        onDoubleClicked: {
                            showEdgePropertiesDialog(edgeItem.edge);
                        }
                    }
                }
            }

            Repeater {
                model: nodeModel
                NodeItem {
                    id: nodeItem
                    node: model.dataRole
                    origin: scene.origin
                    highlighted: addEdgeAction.from == node || addEdgeAction.to == node
                    property bool __modifyingPosition : false
                    Connections {
                        target: selectionRect
                        onChanged: {
                            if (selectMoveAction.checked && selectionRect.contains(x, y)) {
                                highlighted = true
                            } else {
                                highlighted = false
                            }
                        }
                    }
                    Connections {
                        target: scene
                        onUpdateSelection: {
                            highlighted = false
                        }
                        onDeleteSelected: {
                            if (highlighted) {
                                deleteNode(node)
                            }
                        }
                        onCreateEdgeUpdateFromNode: {
                            if (nodeItem.contains(Qt.point(sceneAction.lastMousePressed.x, sceneAction.lastMousePressed.y))) {
                                node.highlighted = true
                                addEdgeAction.from = node
                            }
                        }
                        onCreateEdgeUpdateToNode: {
                            if (nodeItem.contains(Qt.point(sceneAction.lastMouseReleased.x, sceneAction.lastMouseReleased.y))) {
                                node.highlighted = true
                                addEdgeAction.to = node
                            }
                        }
                    }
                    onXChanged: {
                        if (scene.origin != nodeItem.origin
                            || nodeItem.__modifyingPosition
                        ) { // do nothing if item not initialized
                            return;
                        }
                        if (x < 10) {
                            nodeItem.__modifyingPosition = true;
                            var delta = Math.max((10 - x), 10)
                            scene.origin = Qt.point(scene.origin.x - delta, scene.origin.y);
                            scene.width += delta;
                            nodeItem.__modifyingPosition = false;
                            return;
                        }
                        if (x + width + 10 > scene.width) {
                            nodeItem.__modifyingPosition = true;
                            var delta = Math.max(scene.width - (x + width) + 10, 10);
                            scene.width += delta;
                            nodeItem.__modifyingPosition = false;
                            return;
                        }
                    }
                    onYChanged: {
                        if (scene.origin != nodeItem.origin
                            || nodeItem.__modifyingPosition
                        ) { // do nothing if item not initialized
                            return;
                        }
                        if (y < 10) {
                            nodeItem.__modifyingPosition = true;
                            var delta = (10 - y)
                            scene.origin = Qt.point(scene.origin.x, scene.origin.y - delta);
                            scene.height += delta;
                            nodeItem.__modifyingPosition = false;
                            return;
                        }
                        if (y + height + 10 > scene.height) {
                            nodeItem.__modifyingPosition = true;
                            var delta = Math.max(scene.height - (y + height) + 10, 10);
                            scene.height += delta;
                            nodeItem.__modifyingPosition = false;
                            return;
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
                        Loader {
                            id: nodeDialogLoader
                        }
                        onClicked: {
                            if (deleteAction.checked) {
                                deleteNode(nodeItem.node)
                                mouse.accepted = true
                                return
                            }
                            mouse.accepted = false
                        }
                        onDoubleClicked: {
                            showNodePropertiesDialog(nodeItem.node);
                        }
                        onPressed: {
                            if (selectMoveAction.checked && !nodeItem.highlighted) {
                                scene.clearSelection()
                                nodeItem.highlighted = true
                                mouse.accepted = true
                                return
                            }
                            if (!(deleteAction.checked || selectMoveAction.checked)) {
                                mouse.accepted = false
                            }
                            mouse.accepted = false
                        }
                    }
                }
            }
        }
    }

    // state maching solely for select/move
    DSM.StateMachine {
        id: dsmSelectMove
        initialState: smStateIdle
        running: true
        DSM.State {
            id: smStateIdle
            DSM.SignalTransition {
                targetState: smStateSelecting
                signal: sceneAction.onPressed
            }
            DSM.SignalTransition {
                signal: sceneAction.onClicked
                onTriggered: {
                    scene.clearSelection
                }
            }
        }
        DSM.State {
            id: smStateSelecting
            DSM.SignalTransition {
                signal: sceneAction.onPositionChanged
                onTriggered: {
                    selectionRect.visible = true
                    selectionRect.to = sceneAction.lastMousePosition
                }
            }
            DSM.SignalTransition {
                targetState: smStateIdle
                signal: sceneAction.onReleased
            }
            onEntered: {
                scene.clearSelection()
                selectionRect.from = sceneAction.lastMousePressed
            }
            onExited: {
                selectionRect.visible = false
            }
        }
    }

    // state maching solely for edge creation
    DSM.StateMachine {
        id: dsmCreateEdge
        initialState: ceStateIdle
        running: false
        DSM.State {
            id: ceStateIdle
            DSM.SignalTransition {
                targetState: ceStateCreateEdgeTo
                signal: sceneAction.onPressed
            }
            onExited: {
                addEdgeAction.to = null
                scene.createEdgeUpdateFromNode()
            }
        }
        DSM.State {
            id: ceStateCreateEdgeTo
            DSM.SignalTransition {
                targetState: ceStateIdle
                signal: sceneAction.onReleased
            }
            onEntered: {
                createLineMarker.visible = true
            }
            onExited: {
                scene.createEdgeUpdateToNode()
                addEdgeAction.apply()
                createLineMarker.visible = false
            }
        }
    }
}
