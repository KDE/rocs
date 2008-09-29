#include "libgraph_Node.h"
#include "libgraph_Edge.h"
#include "libgraph_Graph.h"

#include <QString>

using namespace libgraph;

Edge::Edge(Node *from, Node *to, Graph *parent) : QObject(parent)
{
  _from = from;
  _to = to;
  _from_disconnected = 0;
  _to_disconnected = 0;
}

Edge::~Edge()
{
  // Remove from '_from', '_to', and the Parent Graph.
  Graph *g = qobject_cast<Graph *>(parent());
  
  g   -> destroyEdge(this);  
  _to -> destroyEdge(this);
  
  if (_to != _from){
    _from -> destroyEdge(this);
  }
}

bool Edge::visited() const
{
  return _visited;
}

void Edge::setVisited(bool visited )
{
  _visited = visited;
  emit visitedChanged(visited);
}

void Edge::unsetVisited()
{
  _visited = false;
  emit visitedChanged(false);
}
QString Edge::name() const
{
  return _name;
}

void Edge::setName(const QString& name)
{
  _name = name;
  emit nameChanged(name);
}

QColor Edge::color() const
{
  return _color;
}

void Edge::setColor(QColor color)
{
  _color = color;
  emit colorChanged(color);
}

qreal Edge::length() const
{
  return _length;
}

void Edge::setLength(const qreal& length)
{
  _length = length;
  emit lengthChanged(length);
}

Node* Edge::from() const
{
  return _from;
}

Node* Edge::to() const
{
  return _to;
}

void Edge::connectNodes()
{
  if ( _to != 0) return;
  _to = _to_disconnected;
  _to_disconnected = 0;
  _from = _from_disconnected;
  _from_disconnected = 0;
}

void Edge::disconnectNodes()
{
  if (_to_disconnected != 0) return;
  _to_disconnected = _to;
  _to = 0;
  _from_disconnected = _from;
  _from = 0;
}
