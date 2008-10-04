#include "libgraph_MultiGraph.h"
#include "libgraph_MultiNode.h"
#include "libgraph_Edge.h"
#include "libgraph_GraphDocument.h"

using namespace libgraph;

MultiGraph::MultiGraph(GraphDocument *parent) : Graph(parent){

}

MultiGraph::~MultiGraph(){

}

Edge* MultiGraph::createEdge(Node* from, Node* to){
  if (( _nodes.indexOf(from) == -1 ) || ( _nodes.indexOf(to) == -1) )  return 0;

  Edge *e = new Edge(from, to, this);
  _edges.append(e);
  from -> addEdge(e);

  if (from != to) to -> addEdge(e);
  
  emit edgeCreated(e);
  return e;
}

Node* MultiGraph::createNode(QPointF position){
  Node *n = new MultiNode(position, this);
  _nodes.append(n);
  n -> setIndex( _nodes.size() - 1 );

  emit nodeCreated(n);
  return n;
}
