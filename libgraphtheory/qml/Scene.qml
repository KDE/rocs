// SPDX-FileCopyrightText: 2014-2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml.StateMachine as DSM
import org.kde.rocs.graphtheory

Item {
    id: root
    width: 800
    height: 600

    focus: true

    // element create/remove actions
    signal createNode(real x, real y, int typeIndex);
    signal createEdge(Node from, Node to, int typeIndex);
    signal deleteNode(Node node);
    signal deleteEdge(Edge edge);

    // exec dialog signals
    signal showNodePropertiesDialog(Node node);
    signal showEdgePropertiesDialog(Edge edge);

    Connections {
        target: selectMoveAction
        function onToggled() {
            dsmSelectMove.running = selectMoveAction.checked
        }
    }
    Connections {
        target: addEdgeAction
        function onToggled() {
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
        case Qt.Key_A: //CTRL+A
            if (event.modifiers & Qt.ControlModifier) {
                scene.selectAll();
                event.accepted = true;
            }
            break;
        default:
            break;
        }
    }

    ButtonGroup {
        id: editToolButton
         buttons: toolbar.children
    }
    ColumnLayout {
        id: toolbar
        ToolButton {
            action: SelectMoveAction {
                id: selectMoveAction
                checked: true
            }
        }
        ToolButton {
            action: AddNodeAction {
                id: addNodeAction
            }
        }
        ToolButton {
            action: AddEdgeAction {
                id: addEdgeAction
                onCreateEdge: root.createEdge(from, to, edgeTypeSelector.currentIndex)
            }
        }
        ToolButton {
            action: DeleteAction {
                id: deleteAction
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
            color: "#ffffff"
            width: parent.width
            height: parent.height
        }

        Item {
            id: scene
            width: sceneScrollView.width - 30
            height: sceneScrollView.height - 20
            z: -10 // must lie behind everything else
            signal deleteSelected();
            signal startMoveSelected();
            signal finishMoveSelected();
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
            function selectAll()
            {
                selectionRect.from = Qt.point(0, 0)
                selectionRect.to = Qt.point(width,height)
                updateSelection();
            }

            MouseArea {
                id: sceneAction
                anchors.fill: parent
                z: -10 // must lie behind everything else
                preventStealing: true

                property point lastMouseClicked: Qt.point(0, 0)
                property point lastMousePressed: Qt.point(0, 0)
                property point lastMouseReleased: Qt.point(0, 0)
                property point lastMousePosition: Qt.point(0, 0)
                property bool nodePressed: false // if true, the current mouse-press event started at a node

                onClicked: {
                    lastMouseClicked = Qt.point(mouse.x, mouse.y)
                    if (addNodeAction.checked) {
                        mouse.accepted = true
                        createNode(mouse.x, mouse.y, nodeTypeSelector.currentIndex);
                        return
                    }
                }
                onPressed: {
                    lastMousePressed = Qt.point(mouse.x, mouse.y)
                    lastMousePosition = Qt.point(mouse.x, mouse.y)
                }
                onPositionChanged: {
                    lastMousePosition = Qt.point(mouse.x, mouse.y)
                }
                onReleased: {
                    lastMouseReleased = Qt.point(mouse.x, mouse.y)
                    sceneAction.nodePressed = false
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
                    z: -1 // edges must be below nodes

                    EdgePropertyItem {
                        anchors.centerIn: parent
                        edge: model.dataRole
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onPressed: {
                            if (deleteAction.checked) {
                                deleteEdge(edge)
                                mouse.accepted = true
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
                    highlighted: addEdgeAction.from === node || addEdgeAction.to === node
                    property bool __modifyingPosition: false
                    property point __moveStartedPosition: Qt.point(0, 0)
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
                            if (selectionRect.contains(x, y)) {
                                highlighted = true
                            } else {
                                highlighted = false
                            }
                        }
                        onDeleteSelected: {
                            if (highlighted) {
                                deleteNode(node)
                            }
                        }
                        onStartMoveSelected: {
                            if (!highlighted) {
                                return
                            }
                            __moveStartedPosition.x = node.x
                            __moveStartedPosition.y = node.y
                            node.x = Qt.binding(function() { return __moveStartedPosition.x + sceneAction.lastMousePosition.x - sceneAction.lastMousePressed.x })
                            node.y = Qt.binding(function() { return __moveStartedPosition.y + sceneAction.lastMousePosition.y - sceneAction.lastMousePressed.y })
                        }
                        onFinishMoveSelected: {
                            if (!highlighted) {
                                return
                            }
                            node.x = __moveStartedPosition.x + sceneAction.lastMousePosition.x - sceneAction.lastMousePressed.x
                            node.y = __moveStartedPosition.y + sceneAction.lastMousePosition.y - sceneAction.lastMousePressed.y
                            __moveStartedPosition.x = 0
                            __moveStartedPosition.y = 0
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
                        if (x < 10) {
                            nodeItem.__modifyingPosition = true;
                            var delta = Math.max((10 - x), 10)
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
                        if (y < 10) {
                            nodeItem.__modifyingPosition = true;
                            var delta = (10 - y)
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
                        onDoubleClicked: {
                            showNodePropertiesDialog(nodeItem.node);
                            mouse.accepted = true
                        }
                        onPressed: {
                            // never handle undefined actions signals
                            if (!(selectMoveAction.checked || deleteAction.checked)) {
                                mouse.accepted = false
                                return
                            }
                            if (deleteAction.checked) {
                                deleteNode(nodeItem.node)
                                mouse.accepted = true
                                return
                            }
                            // single-node move action: directly handle it
                            sceneAction.nodePressed = true
                            if (selectMoveAction.checked && !nodeItem.highlighted) {
                                scene.clearSelection()
                                nodeItem.highlighted = true
                                mouse.accepted = true
                                return
                            }
                            // multi-node move action: gets handled by state-machine
                            if (selectMoveAction.checked && nodeItem.highlighted) {
                                mouse.accepted = false
                                return
                            }
                        }
                        onReleased: {
                            sceneAction.nodePressed = false
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        id: extraToolbarCreateNode
        visible: addNodeAction.checked
        anchors {
            top: sceneScrollView.top
            right:sceneScrollView.right
            topMargin: 10
            rightMargin: 5
        }
        ComboBox {
            id: nodeTypeSelector
            model: nodeTypeModel
            textRole: "titleRole"
        }
    }
    RowLayout {
        id: extraToolbarCreateEdge
        visible: addEdgeAction.checked
        anchors {
            top: sceneScrollView.top
            right:sceneScrollView.right
            topMargin: 10
            rightMargin: 5
        }
        ComboBox {
            id: edgeTypeSelector
            model: edgeTypeModel
            textRole: "titleRole"
        }
    }

    // state machine only for select/move
    DSM.StateMachine {
        id: dsmSelectMove
        initialState: smStateIdle
        running: true
        DSM.State {
            id: smStateIdle
            DSM.SignalTransition {
                targetState: smStateMoving
                signal: sceneAction.onPressed
                guard: sceneAction.nodePressed
            }
            DSM.SignalTransition {
                targetState: smStateSelecting
                signal: sceneAction.onPressed
                guard: !sceneAction.nodePressed
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
        DSM.State {
            id: smStateMoving
            DSM.SignalTransition {
                targetState: smStateIdle
                signal: sceneAction.onReleased
            }
            onEntered: {
                scene.startMoveSelected();
            }
            onExited: {
                scene.finishMoveSelected()
            }
        }
    }

    // state machine solely for edge creation
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
