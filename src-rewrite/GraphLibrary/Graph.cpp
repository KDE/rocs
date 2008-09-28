#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include <iostream>

// Default Constructor
Graph::Graph() : QObject(0)
{
  _nodes.clear();
  _edges.clear();
}

// Default Destructor 
Graph::~Graph()
{
  foreach(Node *n, _nodes)
  {
    delete n;
  }
  _nodes.clear();
  _edges.clear();
}


// Changes the Graph's Name 
void Graph::setName(const QString& name)
{
  _name = name;
  emit nameChanged(name);
}

//  Gets the name of the graph 
QString Graph::name() const
{
  return _name;
}

// Changes the Graph's Color 
void Graph::setColor(QColor color)
{
  _color = color;
  emit colorChanged(color);
}

// Gets the color of the Graph 
QColor Graph::color() const
{
  return _color;
}

// Removes a node
void Graph::removeNode(int index)
{
  if ( index >= _nodes.size() ) return;
  Node *n = _nodes[index];
  delete n;

  _nodes.removeAt(index);
  int size = _nodes.size();
  while (index < size )
  {
    Node *n = _nodes[index];
    n -> setIndex(index);
    index++;
  }
  
  emit nodeRemoved(index);
}

// Remove one node from the list.
void Graph::removeNode(Node *node)
{
  removeNode(_nodes.indexOf(node));
}

// Removes an Edge 
void Graph::removeEdge(int index)
{
  if (index >= _edges.size()) return;
  Edge *e = _edges[index];
  delete e;
}

void Graph::removeEdge(Edge *e)
{
  if (_edges.indexOf(e) == -1) return;
  delete e;
}

void Graph::destroyEdge(Edge *e)
{
    emit edgeRemoved(_edges.indexOf(e));
    _edges.removeOne(e);

}

// Return all the nodes that this graph holds
QList<Node*> Graph::nodes() const
{
  return _nodes;
}

// Return all edgfes that this graph holds
QList<Edge*> Graph::edges() const
{
  return _edges;
}

// Transform this graph into a KGraph
void Graph::transformIntoKGraph()
{
  foreach(Node *n1, _nodes)
  {
    foreach(Node *n2, _nodes)
    {
      if (n1 == n2) continue;
      if (n1 -> isConnected(n2)) continue;
      createEdge(n1, n2);
    }
  }
}
