#include "GraphCollection.h"
#include "Graph.h"
#include "SimpleGraph.h"
#include "MultiGraph.h"
#include "Digraph.h"

#include <QString>

// Default Constructor
GraphCollection::GraphCollection()
{
  _graphs.clear();
}

// Default Destructor
GraphCollection::~GraphCollection()
{
  foreach(Graph *g, _graphs)
  {
    delete g;
  }
  _graphs.clear();
}


// Sets the current file name of the Graph Collection 
void GraphCollection::setName(const QString& name)
{
  _name = name;
  emit nameChanged(name);
}

// Gets the name of the Graph
QString GraphCollection::name() const
{
  return _name;
}

// set the width of the drawable area
void GraphCollection::setWidth(qreal width)
{
  _width = width;
}

//set the height of the drawable area
void GraphCollection::setHeight(qreal height)
{
  _height = height;
}

// gets the wheight of the drawable area
qreal GraphCollection::height() const
{
  return _height;
}

// sets the width of the drawable area
qreal GraphCollection::width() const
{
  return _width;
}
// Gets the size of the graph Collection
int GraphCollection::size()
{
  return _graphs.size();
}

// Gets the Graph at position 'i' on the _graphs list
Graph* GraphCollection::operator[](int i)
{
  if (_graphs.size() <= i) return 0;
  return _graphs[i];
}

// Creates a new Graph
Graph* GraphCollection::addGraph(int type)
{
  Graph *g = 0;
  switch(type)
  {
    case 0 : g = new SimpleGraph(this); break;
    case 1 : g = new MultiGraph(this); break;
    case 2 : g = new SimpleGraph(this); break;
  }
  
  emit graphCreated(g);
  return g;
}

// Append a grpah on the graph's list
void GraphCollection::operator<<(Graph *g)
{
  _graphs << g;
  emit graphCreated(g);
}

// Append a graph on the Graph's list 
void GraphCollection::append(Graph *g)
{
  _graphs << g;
  emit graphCreated(g);
}

// Prepend a graph on the Graph's list
void GraphCollection::prepend(Graph *g)
{
  _graphs.prepend(g);
  emit graphCreated(g);
}

// remove the first instance of the graph G
void GraphCollection::removeOne(Graph *g)
{
  if ( _graphs.indexOf(g) == -1) return;
  removeAt(_graphs.indexOf(g));  
}

// Removes the graph at position 'i'
void GraphCollection::removeAt(int i)
{
  if (i >= _graphs.size()) return;
  Graph *g = _graphs[i];
  _graphs.removeAt(i);
  emit graphRemoved(i);
  delete g;
}

void GraphCollection::clear()
{
  foreach(Graph *g, _graphs){
    delete g;
  }
  _graphs.clear();
}

int GraphCollection::indexOf(Graph *g)
{
  return _graphs.indexOf(g);
}
