#include "libgraph_MultiNode.h"
#include "libgraph_Edge.h"

using namespace libgraph;

MultiNode::MultiNode(QPointF position, Graph *parent) : Node(position, parent){}

MultiNode::~MultiNode(){
  foreach(Edge *e, _edges)  delete e;
  _edges.clear();
}

bool MultiNode::isConnected(Node* to){

  foreach(Edge *e, _edges){
    if ((e->to() == to) && (e->from() == this)) return true;
    if ( (e->from() == to) && (e->to() == this)) return true;
  }

  return false;

}

void MultiNode::addEdge(Edge *e){
  _edges.append(e);
}

// Return the first node that this node is connected to. 
Node* MultiNode::getAdjacentNode(){
  if (_edges.size() == 0)  return 0;
  return getAdjacentNode( _edges[0] );
}

// Return the node connected with this node using edge e.
Node* MultiNode::getAdjacentNode(Edge *e){
  if (_edges.indexOf(e) == -1)  return 0;
  return ( e->from() == this) ? e->to() : e-> from();
}

// Return the first node that has not been visited and this node is connected to 
Node* MultiNode::getFirstUnvisitedNode(){
  foreach(Edge *e, _edges)  {
    Node *n = getAdjacentNode(e);
    if ( !n -> visited() ){
      return n;
    }
  }

  return 0;
}

// Return the first edge on the list, does not care if it's visited or not 
Edge* MultiNode::getFirstEdge(){
  if (_edges.size() == 0)  return 0;
  return _edges[0];
}

// Return the first unvisited edge on the list
Edge* MultiNode::getFirstUnvisitedEdge(){
  foreach(Edge *e, _edges){
    if ( ! e->visited() )  return e;
  }

  return 0;
}
