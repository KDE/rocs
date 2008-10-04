#include "libgraph_GraphDocument.h"
#include "libgraph_Graph.h"
#include "libgraph_SimpleGraph.h"
#include "libgraph_MultiGraph.h"
#include "libgraph_Digraph.h"

#include <QString>

using namespace libgraph;

// Default Constructor
GraphDocument::GraphDocument(const QString name){
  _graphs.clear();
  _name = name;
}

// Default Destructor
GraphDocument::~GraphDocument(){
  foreach(Graph *g, _graphs){
    delete g;
  }
  _graphs.clear();
}


// Sets the current file name of the Graph Collection 
void GraphDocument::setName(const QString& name){
  _name = name;
  emit nameChanged(name);
}

// Gets the name of the Graph
QString GraphDocument::name() const{
  return _name;
}

// set the width of the drawable area
void GraphDocument::setWidth(qreal width){
  _width = width;
}

//set the height of the drawable area
void GraphDocument::setHeight(qreal height){
  _height = height;
}

// gets the wheight of the drawable area
qreal GraphDocument::height() const{
  return _height;
}

// sets the width of the drawable area
qreal GraphDocument::width() const{
  return _width;
}
// Gets the size of the graph Collection
int GraphDocument::size(){
  return _graphs.size();
}

// Gets the Graph at position 'i' on the _graphs list
Graph* GraphDocument::operator[](int i){
  if (_graphs.size() <= i) return 0;
  return _graphs[i];
}

// Gets the Graph at position 'i' on the _graphs list
Graph* GraphDocument::at(int i){
  if (_graphs.size() <= i) return 0;
  return _graphs[i];
}

// Creates a new Graph
Graph* GraphDocument::addGraph(QString name, int type){
  Graph *g = 0;
  switch(type){
    case 0 : g = new SimpleGraph(this); break;
    case 1 : g = new MultiGraph(this); break;
    case 2 : g = new Digraph(this); break;
  }

  g -> setName(name);

  emit graphCreated(g);
  return g;
}

// Append a grpah on the graph's list
void GraphDocument::operator<<(Graph *g){
  _graphs << g;
  emit graphCreated(g);
}

// Append a graph on the Graph's list 
void GraphDocument::append(Graph *g){
  _graphs << g;
  emit graphCreated(g);
}

// Prepend a graph on the Graph's list
void GraphDocument::prepend(Graph *g){
  _graphs.prepend(g);
  emit graphCreated(g);
}

// remove the first instance of the graph G
void GraphDocument::removeOne(Graph *g){
  if ( _graphs.indexOf(g) == -1) return;
  removeAt(_graphs.indexOf(g));  
}

// Removes the graph at position 'i'
void GraphDocument::removeAt(int i){
  if (i >= _graphs.size()) return;
  Graph *g = _graphs[i];
  _graphs.removeAt(i);
  emit graphRemoved(i);
  delete g;
}

void GraphDocument::clear(){
  foreach(Graph *g, _graphs) delete g;
  _graphs.clear();
}

int GraphDocument::indexOf(Graph *g){
  return _graphs.indexOf(g);
}
