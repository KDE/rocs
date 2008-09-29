#include "SimpleNode.h"
#include "Edge.h"

SimpleNode::SimpleNode(QPointF position, Graph *parent) : Node(position, parent)
{

}

SimpleNode::~SimpleNode()
{
  foreach(Edge *e, _edges)
  {
    delete e;
  }
  _edges.clear();
}

bool SimpleNode::isConnected(Node *to)
{
 foreach(Edge *e, _edges)
 {  // Doesn't care the direction.
    if ((e->to() == to) && (e->from() == this)) return true;
    if ((e->from() == to) && (e->to() == this)) return true;
 }
  return false;
}


void SimpleNode::addEdge(Edge *e)
{
  Node *from = e->from();
  Node *to = e->to();

  // Do not insert same edge 2 times.
  foreach(Edge *edge, _edges)
  {
    if ((edge->to() == to) && (edge->from() == from)) return;
    if ((edge->from() == to) && (edge->to() == from))   return;
  }
  _edges.append(e);

}
