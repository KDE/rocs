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

// Return the first node that this node is connected to. 
Node* SimpleNode::getAdjacentNode()
{
  if (_edges.size() == 0)
  {
    return 0;
  }
  return getAdjacentNode( _edges[0] );
}

// Return the node connected with this node using edge e.
Node* SimpleNode::getAdjacentNode(Edge *e)
{
  if (_edges.indexOf(e) == -1)
  {
    return 0;
  }
  return ( e->from() == this) ? e->to() : e-> from();
}

// Return the first node that has not been visited and this node is connected to 
Node* SimpleNode::getFirstUnvisitedNode()
{
  foreach(Edge *e, _edges)
  {
    Node *n = getAdjacentNode(e);
    if ( !n -> visited() ){
      return n;
    }
  }

  return 0;
}

// Return the first edge on the list, does not care if it's visited or not 
Edge* SimpleNode::getFirstEdge()
{
  if (_edges.size() == 0)
  {
    return 0;
  }
  return _edges[0];
}

// Return the first unvisited edge on the list
Edge* SimpleNode::getFirstUnvisitedEdge()
{
  foreach(Edge *e, _edges)
  {
    if ( ! e->visited() )
    {
      return e;
    }
  }

  return 0;
}
