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

GraphScene::GraphScene(QObject *parent) : QGraphicsScene(parent){
	_graphDocument = 0;

}

void GraphScene::setGraph(Graph *g){
  _graph = g;
}

void GraphScene::setAction(AbstractAction *action){
  _action = action;
}

void GraphScene::setGraphDocument(GraphDocument *gd){
  _graphDocument = gd;
}
QGraphicsItem *GraphScene::createNode(Node *n){
  NodeItem *nItem = new NodeItem(n);
  insertGraphItem(nItem);
  return nItem;
}

QGraphicsItem *GraphScene::createEdge(Edge *e){
    QGraphicsItem *edgeItem = 0;
    
    if ( !_graph->directed() ){ 
      edgeItem = new EdgeItem(e); 
    }
    else{
      edgeItem = new OrientedEdgeItem(e);
    }
    
    qreal x1 = e->from()->property("x").toDouble();
    qreal y1 = e->from()->property("y").toDouble();
    qreal x2 = e->to()->property("x").toDouble();
    qreal y2 = e->to()->property("y").toDouble();
    
    NodeItem *nFrom = qgraphicsitem_cast<NodeItem*>(itemAt(x1,y1));
    NodeItem *nTo  = qgraphicsitem_cast<NodeItem*>(itemAt(x2,y2));
    nFrom->addEdge(edgeItem);
    nTo->addEdge(edgeItem);
    insertGraphItem(edgeItem);
    return edgeItem;
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executePress(mouseEvent->scenePos());
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executeRelease(mouseEvent->scenePos());
}

void GraphScene::wheelEvent(QGraphicsSceneWheelEvent *){

}

void GraphScene::keyPressEvent(QKeyEvent *){

}

void GraphScene::insertGraphItem(QGraphicsItem *item){
  addItem(item);
  _hashGraphs.insert(_graph, item);
}

void GraphScene::updateGraph(Graph *g){
  QList<QGraphicsItem*> items = _hashGraphs.values(g);
  foreach(QGraphicsItem *i, items){
    removeItem(i);
  }
  _hashGraphs.values(g);
  _hashGraphs.remove(g);

  QList<Node*> nodes = g->nodes();
  foreach(Node *n, nodes){
    createNode(n);
  }
  
  QList<Edge*> edges = g->edges();
  foreach(Edge *e, edges){
    createEdge(e);
  }

 /* QList<Group*> groups = g->groups();
  foreach(Group* g, groups){
    
  }
*/
}

void GraphScene::updateDocument(){
	if (_graphDocument == 0){
	    kDebug() << "Graph Document is null. please hit the developer.";
	    return;
	}
	
	kDebug() << "Graph Document Size: " << _graphDocument->size();
	int size = _graphDocument->size();
	for(int i = 0; i < size; i++){
		
		updateGraph( _graphDocument->at(i) );	
	}
}