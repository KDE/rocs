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
#include "graph.h"

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
    _graphDocument = 0;
 _hideEdges = false;
}

bool GraphScene::hideEdges(){
  return _hideEdges;
}

void GraphScene::setHideEdges(bool h){
  _hideEdges = h;
  if ( ! _hideEdges ){
    foreach(QGraphicsItem *i, _hidedEdges){
      i->update();
    }
  }
}

void GraphScene::setActiveGraph(Graph *g) {
    kDebug() << "Active Graph Setted";
    _graph = g;
}

void GraphScene::updateAfter(QGraphicsItem *item){
  if (_hidedEdges.contains(item)) return;
  _hidedEdges << item;
}


void GraphScene::setAction(AbstractAction *action) {
    _action = action;
}

void GraphScene::setActiveGraphDocument(GraphDocument *gd) {
    _graphDocument = gd;
    if (gd == 0) {
        kDebug() << "Graph Document Doesn't Exist, crashing.";
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

}
QGraphicsItem *GraphScene::createNode(Graph *g, Node *n) {
    NodeItem *nItem = new NodeItem(n);
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

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executePress(mouseEvent->scenePos());

}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeRelease(mouseEvent->scenePos());
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *) {

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

void GraphScene::removeGItem(QGraphicsItem *i){
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
        createNode(g, n);
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
