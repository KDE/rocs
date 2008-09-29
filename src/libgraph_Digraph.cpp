#include "libgraph_Digraph.h"
#include "libgraph_DiNode.h"
#include "libgraph_Edge.h"
#include "libgraph_GraphDocument.h"

#include <QPointF>

using namespace libgraph;

Digraph::Digraph(GraphDocument *parent) : Graph(parent)
{

}

Digraph::~Digraph()
{

}

Edge* Digraph::addEdge(Node *from, Node *to)
{
  if (( _nodes.indexOf(from) == -1 ) || ( _nodes.indexOf(to) == -1) )
  {
    return 0;
  }

  Edge *e = new Edge(from, to, this);

  from -> addEdge(e);
  if  (from != to)
  {
    to -> addEdge(e);
  }

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
