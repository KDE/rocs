#include "OrientedNode.h"
#include "Edge.h"
#include "OrientedGraph.h"
#include <KDebug>


bool OrientedNode::isAdjacent(Node* n){  
 foreach(Edge *e, _edges){
    if ( e->to() == n) return true;
  }
  return false;
}

QList<Node*> OrientedNode::adjNodes(){
  QList<Node*> tmpList;
  foreach(Edge *e, _edges){
    if (e->from() == e->to()) tmpList.append(this);
    if (e->to() != this) tmpList.append(e->to());
  }
  return tmpList;
}

Node* OrientedNode::adjNode(){
  foreach( Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    if ( e->to() != this) return e->to();
  }
  return 0;
}

Node* OrientedNode::unvisitedAdjNode(){
  foreach( Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    if (( e->to() != this) && ( ! e->to()->visited() )) return e->to();
  }
  return 0;
}

QList<Edge*> OrientedNode::adjEdges(Node *n){
  QList<Edge*> tmpEdges;
  if (n == 0) foreach(Edge *e, _edges){
    if ( e->from() == e->to()) tmpEdges.append(e);
    if ( e->from() == this) tmpEdges.append(e);
  }
  else foreach(Edge *e, _edges){
    if ( e->to() == n) tmpEdges.append(e);
  }
  qSort(tmpEdges.begin(), tmpEdges.end());
  return tmpEdges;
}

Edge* OrientedNode::unvisitedAdjEdge(){
  if (_edges.size() == 0) return 0;
  foreach(Edge *e, _edges){
    if (( e->from() == this) && ( ! e->visited())) return e;
  }
  return 0;
}

Edge* OrientedNode::adjEdge(){
  foreach(Edge *e, _edges){
    if (e->from() == this) return e;
  }
  return 0;
}

Edge* OrientedNode::connect(Node* n){
  Graph *g = qobject_cast<Graph*>(parent()); 
  if (! g->checkExistence(n) ) return 0;
  Edge *e = 0;
  e = new Edge(this, n, g);
    
  kDebug() << "Oriented Edge Created";
  return e;
}