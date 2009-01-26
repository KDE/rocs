#include "SUI_GraphScene.h"
#include "action_AbstractAction.h"

#include "node.h"
#include "edge.h"
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
	
}

void GraphScene::setGraph(Graph *g){
  _graph = g;
}

void GraphScene::setAction(AbstractAction *action){
  _action = action;
}

NodeItem *GraphScene::createNode(Node *n){
  NodeItem *nItem = new NodeItem(n);
  insertGraphItem(nItem);
  return nItem;
}

EdgeItem *GraphScene::createEdge(Edge *e){
  EdgeItem *eItem = new EdgeItem(e);
  insertGraphItem(eItem);
  return eItem;
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
    _hashGraphs.insert(g, createNode(n));
  }
  
  QList<Edge*> edges = g->edges();
  
  foreach(Edge *e, edges){
    QGraphicsItem *edgeItem = 0;
    
    if ( !_graph->directed() ){ 
      edgeItem = new EdgeItem(e); 
      kDebug()  << "Criou Edge nao orientado";
    }
    else{ 
      edgeItem = new OrientedEdgeItem(e);
      kDebug() << "Criou Edge Orientado";
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
  }

 /* QList<Group*> groups = g->groups();
  foreach(Group* g, groups){
    
  }
*/

}
