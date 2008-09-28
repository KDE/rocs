#include "Digraph.h"
#include "DiNode.h"
#include "Edge.h"
#include <QPointF>

Digraph::Digraph() : Graph()
{

}

Digraph::~Digraph()
{

}

Edge* Digraph::addEdge(Node *from, Node *to)
{
  Edge *e = new Edge(from, to, this);
  from -> addEdge(e);
  emit edgeCreated(e);
  return e;
}

Node* Digraph::createNode(QPointF position)
{
  Node *n = new DiNode(position, this);

  _nodes.append(n);

  n -> setIndex( _nodes.size() - 1 );

  emit nodeCreated(n);

  return n;
}
