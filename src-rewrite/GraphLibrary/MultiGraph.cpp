#include "MultiGraph.h"
#include "MultiNode.h"
#include "Edge.h"

MultiGraph::MultiGraph() : Graph()
{

}

MultiGraph::~MultiGraph()
{

}

Edge* MultiGraph::createEdge(Node* from, Node* to)
{
  Edge *e = new Edge(from, to, this);
  _edges.append(e);
  
  from -> addEdge(e);
  
  if (from != to)
  {
    to -> addEdge(e);
  }
  emit edgeCreated(e);
  return e;
}

Node* MultiGraph::createNode(QPointF position)
{
  Node *n = new MultiNode(position, this);

  _nodes.append(n);

  n -> setIndex( _nodes.size() - 1 );

  emit nodeCreated(n);

  return n;
}
