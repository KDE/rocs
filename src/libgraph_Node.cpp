#include "libgraph_Node.h"
#include "libgraph_Graph.h"
#include "libgraph_Edge.h"
#include <QPointF>
#include <QColor>
#include <QString>

using namespace libgraph;

// Default Constructor
Node::Node( QPointF position, Graph *parent ) : QObject(parent)
{
  _position = position;
  _edges.clear();
}
  
// Default Destructor 
Node::~Node()
{
  // Must remove each edge that this node is connected to.
  foreach(Edge *e, _edges)
  {
    delete e;
  }
  _edges.clear();
}

// gets the name of the Node 
QString Node::name() const
{
  return _name;
}

// removes the edge from this node.
void Node::destroyEdge(Edge *e)
{
  _edges.removeOne(e);
}

void Node::removeEdge(Edge *e)
{
  if ( _edges.indexOf(e) == -1) return;
  delete e;
}

void Node::removeEdge(int index)
{
  if (index >= _edges.size()) return;
  Edge *e = _edges[index];
  delete e;
}

// sets the name of the node 
void Node::setName(QString name)
{
  _name = name;
  emit nameChanged(name);
}

// gets the position of the node 
QPointF Node::position() const
{
  return _position;
}

// sets the position of the node 
void Node::setPosition(QPointF position)
{
  _position = position;
  emit positionChanged(position);
}

// gets the node's color 
QColor Node::color() const
{
  return _color;
}
  
// sets the Node's color 
void Node::setColor(QColor color)
{
  _color = color;
  emit colorChanged(color);
}

// gets the node Index 
int Node::index() const
{
  return _index;
}

//sets the node Index 
void Node::setIndex(int index)
{
  _index = index;
  emit indexChanged(index);
}

QList<Edge*> Node::edges() const
{
  return _edges;
}

// checks if the node is or isn't visited 
bool Node::visited()
{
  return _visited;
}

// set the visited to true 
void Node::setVisited(bool visited)
{
  if (visited == _visited)
  {
    return;
  }

  _visited = visited;
  emit visitedChanged(visited);
}

// unset the visited node 
void Node::unsetVisited()
{
  if (_visited == false)
  {
    return;
  }

  _visited = false;
  emit visitedChanged(false);
}
