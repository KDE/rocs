// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.StateMachine as DSM
import org.kde.rocs

Item {
    id: root
    width: 800
    height: 600

    focus: true

    required property Graph graph
    required property EdgeModel edgeModel
    required property NodeModel nodeModel
    required property EdgeTypeModel edgeTypeModel
    required property NodeTypeModel nodeTypeModel

    // exec dialog signals
    signal showNodePropertiesDialog(Node node);
    signal showEdgePropertiesDialog(Edge edge);

    Keys.onPressed: event => {
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
        exclusive: true
    }
    ColumnLayout {
        id: toolbar
        ToolButton {
            id: selectMoveToolButton
            text: i18n("Select & Move")
            icon.source: "qrc:/icons/select"
            ToolTip.text: i18n("Select and move elements on the scene")
            checkable: true
            checked: true
            ButtonGroup.group: editToolButton
        }
        ToolButton {
            id: addNodeToolButton
            text: i18n("Create Node")
            icon.source: "qrc:/icons/node"
            checkable: true
            ToolTip.text: i18n("Add a node to the scene")
            ButtonGroup.group: editToolButton
        }
        ToolButton {
            id: addEdgeToolButton
            property Node from: null
            property Node to: null

            function apply() {
                if (from && to) {
                    root.graph.createEdge(from, to, edgeTypeSelector.currentValue)
                }
                from = null;
                to = null;
            }

            text: i18n("Create Edge")
            icon.source: "qrc:/icons/edge"
            checkable: true
            ToolTip.text: i18n("Create an edge between two nodes")
            ButtonGroup.group: editToolButton
        }
        ToolButton {
            id: deleteToolButton
            text: i18n("Delete Element")
            icon.source: "qrc:/icons/delete"
            checkable: true
            ToolTip.text: i18n("Delete element from scene")
            ButtonGroup.group: editToolButton
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

        Item {
            id: scene
            width: sceneScrollView.width - 30
            height: sceneScrollView.height - 20
            z: -10 // must lie behind everything else
            signal deleteSelected();
            signal startMoveSelected();
            signal finishMoveSelected();
            signal createEdgeUpdateFromNode();
            signal createEdgeUpdateToNode();
            function clearSelection()
            {
                selectionArea.from = Qt.point(0, 0)
                selectionArea.to = Qt.point(0, 0)
                selectionArea.visible = false
                selectionArea.selecting = false
            }
            function setEdgeFromNode()
            {
                addEdgeToolButton.to = null
                createEdgeUpdateFromNode();
            }
            function selectAll()
            {
                selectionArea.from = Qt.point(0, 0)
                selectionArea.to = Qt.point(width,height)
                selectionArea.visible = false
                selectionArea.selecting = true
            }

            MouseArea {
                id: sceneAction
                anchors.fill: parent
                z: -10 // must lie behind everything else
                preventStealing: true

                signal pressStarted()

                property point lastMouseClicked: Qt.point(0, 0)
                property point lastMousePressed: Qt.point(0, 0)
                property point lastMouseReleased: Qt.point(0, 0)
                property point lastMousePosition: Qt.point(0, 0)
                property bool nodePressed: false // if true, the current mouse-press event started at a node

                onClicked: mouse => {
                    lastMouseClicked = Qt.point(mouse.x, mouse.y)
                    if (addNodeToolButton.checked) {
                        mouse.accepted = true
                        root.graph.createNode(mouse.x, mouse.y, nodeTypeSelector.currentValue);
                        return
                    }
                }
                onPressed: mouse => {
                    lastMousePressed = Qt.point(mouse.x, mouse.y)
                    lastMousePosition = Qt.point(mouse.x, mouse.y)
                    pressStarted()
                }
                onPositionChanged: mouse => {
                    lastMousePosition = Qt.point(mouse.x, mouse.y)
                }
                onReleased: mouse => {
                    lastMouseReleased = Qt.point(mouse.x, mouse.y)
                    sceneAction.nodePressed = false
                }
            }

            GraphSelectionRectangle {
                id: selectionArea
                property bool selecting: false // indicates if nodes shall evaluate
                visible: false
                from: Qt.point(0, 0)
                to: Qt.point(0, 0)
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
                model: root.edgeModel
                EdgeItem {
                    id: edgeItem
                    required edge
                    z: -1 // edges must be below nodes

                    EdgePropertyItem {
                        anchors.centerIn: parent
                        edge: edgeItem.edge
                    }

                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onPressed: mouse => {
                            if (deleteToolButton.checked) {
                                root.graph.deleteEdge(edgeItem.edge)
                                mouse.accepted = true
                            }
                        }
                        onDoubleClicked: {
                            root.showEdgePropertiesDialog(edgeItem.edge);
                        }
                    }
                }
            }

            Repeater {
                model: root.nodeModel
                delegate: NodeItem {
                    id: nodeItem
                    required node
                    highlighted: addEdgeToolButton.from === node || (addEdgeToolButton.from !== null && addEdgeToolButton.checked && contains(sceneAction.lastMousePosition))
                                 || (selectionArea.selecting && selectionArea.contains(x, y))
                                 || Drag.active

                    MouseArea {
                        anchors.fill: parent
                        onPressed: mouse => {
                            if (deleteToolButton.checked) {
                                root.graph.deleteNode(nodeItem.node)
                                mouse.accepted = true
                            }
                            mouse.accepted = false
                        }
                        onDoubleClicked: mouse => {
                            root.showNodePropertiesDialog(nodeItem.node);
                            mouse.accepted = true
                        }
                    }

                    Connections {
                        target: scene
                        function onDeleteSelected() {
                            if (nodeItem.highlighted) {
                                root.graph.deleteNode(nodeItem.node)
                            }
                        }
                        function onCreateEdgeUpdateFromNode() {
                            if (nodeItem.contains(Qt.point(sceneAction.lastMousePressed.x, sceneAction.lastMousePressed.y))) {
                                addEdgeToolButton.from = nodeItem.node
                            }
                        }
                        function onCreateEdgeUpdateToNode() {
                            if (nodeItem.contains(Qt.point(sceneAction.lastMouseReleased.x, sceneAction.lastMouseReleased.y))) {
                                addEdgeToolButton.to = nodeItem.node
                            }
                        }
                    }

                    NodePropertyItem {
                        anchors.centerIn: parent
                        node: nodeItem.node
                    }

                    Drag.active: dragArea.drag.active
                    MouseArea {
                        id: dragArea
                        anchors.fill: parent
                        propagateComposedEvents: true
                        drag.target: { // only enable drag when move/select checked
                            selectMoveToolButton.checked ? parent : undefined
                        }
                        Loader {
                            id: nodeDialogLoader
                        }
                        onDoubleClicked: mouse => {
                            root.showNodePropertiesDialog(nodeItem.node);
                            mouse.accepted = true
                        }
                        onPressed: mouse => {
                            // never handle undefined actions signals
                            if (!(selectMoveToolButton.checked || deleteToolButton.checked)) {
                                mouse.accepted = false
                                return
                            }
                            if (deleteToolButton.checked) {
                                root.graph.deleteNode(nodeItem.node)
                                mouse.accepted = true
                                return
                            }
                            // single-node move action: directly handle it
                            sceneAction.nodePressed = true
                            if (selectMoveToolButton.checked && !nodeItem.highlighted) {
                                scene.clearSelection()
                                mouse.accepted = true
                                return
                            }
                            // multi-node move action: gets handled by state-machine
                            if (selectMoveToolButton.checked && nodeItem.highlighted) {
                                mouse.accepted = false
                                return
                            }
                        }
                        onReleased: mouse => {
                            sceneAction.nodePressed = false
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        id: extraToolbarCreateNode
        visible: addNodeToolButton.checked
        anchors {
            top: sceneScrollView.top
            right:sceneScrollView.right
            topMargin: 10
            rightMargin: 5
        }
        ComboBox {
            id: nodeTypeSelector
            model: root.nodeTypeModel
            textRole: "title"
            valueRole: "id"
            Component.onCompleted: currentIndex = 0
        }
    }
    RowLayout {
        id: extraToolbarCreateEdge
        visible: addEdgeToolButton.checked
        anchors {
            top: sceneScrollView.top
            right:sceneScrollView.right
            topMargin: 10
            rightMargin: 5
        }
        ComboBox {
            id: edgeTypeSelector
            model: root.edgeTypeModel
            textRole: "title"
            valueRole: "id"
            Component.onCompleted: currentIndex = 0
        }
    }

    // state machine only for select/move
    DSM.StateMachine {
        id: dsmSelectMove
        initialState: smStateIdle
        running: selectMoveToolButton.checked
        DSM.State {
            id: smStateIdle
            DSM.SignalTransition {
                targetState: smStateMoving
                signal: sceneAction.pressStarted
                guard: sceneAction.nodePressed
            }
            DSM.SignalTransition {
                targetState: smStateSelecting
                signal: sceneAction.pressStarted
                guard: !sceneAction.nodePressed
            }
            DSM.SignalTransition {
                signal: sceneAction.clicked
                onTriggered: {
                    scene.clearSelection
                }
            }
        }
        DSM.State {
            id: smStateSelecting
            DSM.SignalTransition {
                signal: sceneAction.positionChanged
                onTriggered: {
                    if (!sceneAction.nodePressed && selectionArea.selecting === false) {
                        selectionArea.selecting = true
                        selectionArea.visible = true
                        selectionArea.from = sceneAction.lastMousePosition
                    }
                    selectionArea.to = sceneAction.lastMousePosition
                }
            }
            DSM.SignalTransition {
                targetState: smStateIdle
                signal: sceneAction.released
            }
            onEntered: {
                scene.clearSelection()
                selectionArea.from = sceneAction.lastMousePressed
            }
            onExited: {
                selectionArea.visible = false
            }
        }
        DSM.State {
            id: smStateMoving
            DSM.SignalTransition {
                targetState: smStateIdle
                signal: sceneAction.released
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
        running: addEdgeToolButton.checked
        DSM.State {
            id: ceStateIdle
            DSM.SignalTransition {
                targetState: ceStateCreateEdgeTo
                signal: sceneAction.pressStarted
            }
            onExited: {
                addEdgeToolButton.to = null
                scene.createEdgeUpdateFromNode()
            }
        }
        DSM.State {
            id: ceStateCreateEdgeTo
            DSM.SignalTransition {
                targetState: ceStateIdle
                signal: sceneAction.released
            }
            onEntered: {
                createLineMarker.visible = true
            }
            onExited: {
                scene.createEdgeUpdateToNode()
                addEdgeToolButton.apply()
                createLineMarker.visible = false
            }
        }
    }
}
