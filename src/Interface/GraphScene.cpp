/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "GraphScene.h"
#include "AbstractAction.h"

#include "node.h"
#include "edge.h"
#include "graphDocument.h"
#include "NodeItem.h"
#include "EdgeItem.h"
#include "OrientedEdgeItem.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <KDebug>
#include "node.h"
#include "graph.h"
#include "NodePropertiesWidget.h"
#include "MainWindow.h"
#include "edgepropertieswidget.h"

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
    _graphDocument = 0;
    _hideEdges = false;
    _nodePropertiesWidget = new NodePropertiesWidget(qobject_cast<MainWindow*>(parent));
    _edgePropertiesWidget = new EdgePropertiesWidget();
}

bool GraphScene::hideEdges() {
    return _hideEdges;
}

void GraphScene::setHideEdges(bool h) {
    _hideEdges = h;
    if ( ! _hideEdges ) {
        foreach(QGraphicsItem *i, _hidedEdges) {
            i->update();
        }
    }
}

void GraphScene::setActiveGraph(Graph *g) {
    kDebug() << "Active Graph Setted";
    _graph = g;
}

void GraphScene::updateAfter(QGraphicsItem *item) {
    if (_hidedEdges.contains(item)) return;
    _hidedEdges << item;
}


void GraphScene::setAction(QAction *action) {
    _action = qobject_cast<AbstractAction*>(action);
}

void GraphScene::setActiveGraphDocument(GraphDocument *gd) {
    _graphDocument = gd;
    if (gd == 0) {
        return;
    }

    setSceneRect(QRectF(0,0, gd->width(), gd->height() ));
    QGraphicsRectItem *n = new QGraphicsRectItem(0,0, gd->width(), gd->height());
    n->setFlag(QGraphicsItem::ItemIsSelectable, false);
    n->setZValue(-1000);
    addItem(n);

    int size = _graphDocument->size();

    for (int i = 0; i < size; i++) {
        kDebug() << "Updating Graph at position: " << i;
        updateGraph(_graphDocument->at(i));
        kDebug() << "Graph Updated.";
    }
    kDebug() << "Graph Document Setted" << _graphDocument -> name();
}

QGraphicsItem *GraphScene::createNode(Graph *g, Node *n, bool f) {
    NodeItem *nItem = new NodeItem(n, 0, f);
    insertGraphItem(g, nItem);
    return nItem;
}

QGraphicsItem *GraphScene::createEdge(Graph *g, Edge *e) {
    QGraphicsItem *edgeItem = 0;

    if ( !g->directed() )  edgeItem = new EdgeItem(e);
    else         edgeItem = new OrientedEdgeItem(e);

    insertGraphItem(g, edgeItem);
    return edgeItem;
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
    NodeItem *nitem = qgraphicsitem_cast<NodeItem*>(itemAt(wheelEvent->scenePos()));
    if (!nitem) {
        wheelEvent->ignore();
        return;
    }
    Node *movableNode = nitem->node();
    int numDegrees = wheelEvent->delta();
    if (wheelEvent->orientation() == Qt::Vertical) {
        if (numDegrees > 0)
        {
            nitem->startUpSizing();
            movableNode->setWidth(movableNode->width()+0.25);
            nitem->update();
            nitem->endUpSizing();
        }
        else if (movableNode->width() > 0.5)
        {
            nitem->startDownSizing();
            movableNode->setWidth(movableNode->width()-0.25);
            nitem->update();
            nitem->endDownSizing();
        }
    }
    wheelEvent->accept();
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    if (mouseEvent->button() == 4) {
        NodeItem *nitem = qgraphicsitem_cast<NodeItem*>(itemAt(mouseEvent->scenePos()));
        if (!nitem) return;
        Node *movableNode = nitem->node();
        nitem->startDownSizing();
        movableNode->setWidth(1);
        nitem->update();
        nitem->endDownSizing();
    }
    _action->executePress(mouseEvent->scenePos());
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeRelease(mouseEvent->scenePos());
    if (selectedItems().size() == 1 && _action->name() == "select") {
        NodeItem *nItem = qgraphicsitem_cast<NodeItem*>(selectedItems().at(0));
        if (nItem) {
            _nodePropertiesWidget->setNode(nItem);
            return;
        }
        EdgeItem *eItem = qgraphicsitem_cast<EdgeItem*>(selectedItems().at(0));
        if (eItem) {
            _edgePropertiesWidget->setEdge(eItem->edge());
            return;
        }
        OrientedEdgeItem *oItem = qgraphicsitem_cast<OrientedEdgeItem*>(selectedItems().at(0));
        if (oItem) {
            _edgePropertiesWidget->setEdge(oItem->edge());
            return;
        }
    }
}

void GraphScene::keyPressEvent(QKeyEvent *) {

}

void GraphScene::insertGraphItem(Graph *g, QGraphicsItem *item) {
    addItem(item);
    _hashGraphs.insert(g, item);
}

void GraphScene::clearGraph() {
    int i = _graphDocument->size();
    for (int e = 0; e < i; e++) {
        Graph *g = _graphDocument->at(e);
        QList<QGraphicsItem*> items = _hashGraphs.values(g);
        foreach(QGraphicsItem *i, items) {
            removeItem(i);
            delete i;
        }
        _hashGraphs.remove(g);
    }
}

void GraphScene::removeGItem(QGraphicsItem *i) {
    _hashGraphs.remove(_graph, i);
    removeItem(i);
}

void GraphScene::updateGraph(Graph *g) {
    QList<QGraphicsItem*> items = _hashGraphs.values(g);
    foreach(QGraphicsItem *i, items) {
        removeItem(i);
        delete i;
    }

    _hashGraphs.remove(g);

    QList<Node*> nodes = g->nodes();
    foreach(Node *n, nodes) {
        createNode(g, n, false);
    }

    QList<Edge*> edges = g->edges();
    foreach(Edge *e, edges) {
        createEdge(g, e);
    }
}

void GraphScene::updateDocument() {
    if (_graphDocument == 0) {
        kDebug() << "Graph Document is null. please hit the developer.";
        return;
    }

    kDebug() << "Graph Document Size: " << _graphDocument->size();
    int size = _graphDocument->size();
    for (int i = 0; i < size; i++) {
        updateGraph( _graphDocument->at(i) );
    }
}
