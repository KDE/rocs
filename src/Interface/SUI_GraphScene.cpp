#include "SUI_GraphScene.h"
#include "action_AbstractAction.h"

#include "node.h"
#include "edge.h"
#include "graphDocument.h"
#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>
#include <KDebug>
#include "graph.h"

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent) {
    _graphDocument = 0;
 
}

void GraphScene::setActiveGraph(Graph *g) {
    kDebug() << "Active Graph Setted";
    _graph = g;
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
    n->setZValue(-1);
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
    connect(n, SIGNAL(removed()), nItem, SLOT(remove()));
    insertGraphItem(g, nItem);
    return nItem;
}

QGraphicsItem *GraphScene::createEdge(Graph *g, Edge *e) {
    QGraphicsItem *edgeItem = 0;

    if ( !g->directed() ) {
        edgeItem = new EdgeItem(e);
    }
    else {
        edgeItem = new OrientedEdgeItem(e);
    }

    qreal x1 = e->from()->x(), y1 = e->from()->y();
    qreal x2 = e->to()->x(),  y2 = e->to()->y();

    NodeItem *nFrom = qgraphicsitem_cast<NodeItem*>(itemAt(x1,y1));
    NodeItem *nTo  = qgraphicsitem_cast<NodeItem*>(itemAt(x2,y2));
    nFrom->addEdge(edgeItem);
    nTo->addEdge(edgeItem);
    insertGraphItem(g, edgeItem);
    return edgeItem;
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) {
    _action->executePress(mouseEvent->scenePos());
    //QGraphicsScene::mousePressEvent(mouseEvent);
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
