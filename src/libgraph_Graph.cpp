/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "libgraph_Graph.h"
#include "libgraph_Node.h"
#include "libgraph_Edge.h"
#include "libgraph_GraphDocument.h"

#include <iostream>
#include <math.h>

using namespace libgraph;

// Default Constructor
Graph::Graph(GraphDocument *parent) : QObject(parent){
  GraphDocument *gc = qobject_cast<GraphDocument*>(parent);
  if (gc == 0) return;
  if (gc->indexOf(this) == -1)  gc->append(this);

  _nodes.clear();
  _edges.clear();
}

// Default Destructor 
Graph::~Graph(){
  foreach(Node *n, _nodes)  delete n;
  _nodes.clear();
  _edges.clear();
}

Graph::Graph(const Graph& graph) : QObject( graph.parent() )
{
  
}

// Changes the Graph's Name 
void Graph::setName(const QString& name){
  _name = name;
  emit nameChanged(name);
}

//  Gets the name of the graph 
QString Graph::name() const{
  return _name;
}

// Changes the Graph's Color 
void Graph::setColor(QColor color){
  _color = color;
  emit colorChanged(color);
}

// Gets the color of the Graph 
QColor Graph::color() const{
  return _color;
}

// Removes a node
void Graph::removeNode(int index){
  if ( index >= _nodes.size() ) return;
  Node *n = _nodes[index];
  delete n;

  _nodes.removeAt(index);
  int size = _nodes.size();
  while (index < size ){
    Node *n = _nodes[index];
    n -> setIndex(index);
    index++;
  }
  
  emit nodeRemoved(index);
}

// Remove one node from the list.
void Graph::removeNode(Node *node){
  removeNode(_nodes.indexOf(node));
}

// Removes an Edge 
void Graph::removeEdge(int index){
  if (index >= _edges.size()) return;
  Edge *e = _edges[index];
  delete e;
}

void Graph::removeEdge(Edge *e){
  if (_edges.indexOf(e) == -1) return;
  delete e;
}

void Graph::destroyEdge(Edge *e){
    emit edgeRemoved(_edges.indexOf(e));
    _edges.removeOne(e);
}

// Return all the nodes that this graph holds
QList<Node*> Graph::nodes() const{
  return _nodes;
}

// Return all edgfes that this graph holds
QList<Edge*> Graph::edges() const{
  return _edges;
}

// Transform this graph into a KGraph
void Graph::transformIntoKGraph(){
  foreach(Node *n1, _nodes){
    foreach(Node *n2, _nodes){
      if (n1 == n2) continue;
      if (n1 -> isConnected(n2)) continue;
      createEdge(n1, n2);
    }
  }
}

// creates a KGraph.
int Graph::makeConcentricNodes(int k, qreal radius,  QPointF pos ){
   
  GraphDocument *gc = qobject_cast<GraphDocument*>(parent());
 
  // sees if there's space on the canvas

  if ( (pos.x() - radius) < 0)  pos.setX(radius+20); 
  if ( (pos.y() - radius) < 0)  pos.setY(radius+20); 
  if ( (pos.x() + radius) > gc -> width())  pos.setX( gc->width() - radius - 20);    
  if ( (pos.y() + radius) > gc -> height()) pos.setY( gc->height() - radius - 20); 
  
  qreal x = pos.x();
  qreal y = pos.y();

  int beginOfGraph = _nodes.size();
  for (int i = 1; i <= k; i++) {  
    const qreal angle =  i * (2*M_PI / k);    
    createNode( QPointF(radius * cos(angle) + x, radius * sin(angle) + y) ) ; 
  }
  return beginOfGraph;
}

void Graph::makeKGraph(int k, qreal radius,  QPointF pos){
   int posBegin = makeConcentricNodes(k, radius, pos);
   int s = _nodes.size();

   for(int i = posBegin; i < (s - 1); i++){
     for(int e = posBegin + 1; e < s; e++){
        if (i == e) continue;
        createEdge(_nodes[i], _nodes[e]);
     }
  }
}

void Graph::makeCGraph(int k, qreal radius,  QPointF pos){
   int posBegin = makeConcentricNodes(k, radius, pos);
   int s = _nodes.size();
   int i = 0;
   for(i = posBegin; i < (s - 1); i++) {
     createEdge(_nodes[i], _nodes[i+1]);
   }
   createEdge(_nodes[i], _nodes[posBegin]);
}

QString Graph::toString() const{
  return _name;
}

Node* Graph::createNode(QPointF pos){
  return 0;
}

Edge* Graph::createEdge(Node *from, Node *to){
  return 0;
}

