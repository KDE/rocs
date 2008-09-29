#include "MultiNode.h"
#include "Edge.h"

MultiNode::MultiNode(QPointF position, Graph *parent) : Node(position, parent)
{
}

MultiNode::~MultiNode()
{
  foreach(Edge *e, _edges)
  {
    delete e;
  }
  _edges.clear();
}

bool MultiNode::isConnected(Node* to)
{

  foreach(Edge *e, _edges)
  {
    if ((e->to() == to) && (e->from() == this)) return true;
    if ( (e->from() == to) && (e->to() == this)) return true;
  }

  return false;

}

void MultiNode::addEdge(Edge *e)
{
  _edges.append(e);
}
