#include "DiNode.h"
#include "Graph.h"
#include "Edge.h"

DiNode::DiNode(QPointF position, Graph *parent) 
: Node(position, parent)
{
}

DiNode::~DiNode()
{
  foreach(Edge *e, _edges)
  {
    delete e;
  }
  _edges.clear();
}

bool DiNode::isConnected(Node *to)
{
  foreach(Edge *e, _edges)
  {
    if (e -> to() == to) return true;
  }
  return false;
}

void DiNode::addEdge(Edge *e)
{
  _edges.append(e);
}
