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

#include "Graph.h"
#include "Node.h"
#include "Edge.h"
#include "GraphDocument.h"
#include <math.h>
#include <KDebug>

Graph::Graph(GraphDocument *parent) : QObject(parent){
  GraphDocument *gc = qobject_cast<GraphDocument*>(parent);
  gc->append(this);
  _simple = true;
  _name = "untitled";
  _color = Qt::darkGreen;
}

Graph::Graph( Graph& g) : QObject(g.parent()){
  
  _simple = g.simple();
  _name = g.name();
  _color = g.color();

  foreach(Node *n, g.nodes()){
    Node *tmpNode = addNode(0,0);
    tmpNode->setPos(n->pos());
    tmpNode->setColor(n->color());
    tmpNode->setVisited(n->visited());
    tmpNode->setValue(n->value());
    tmpNode->setTotal(n->total());
    tmpNode->setName(n->name());
  }

  QList< QList < int > > matrix = g.ajdMatrix();
  int matrixSize = matrix.size();
  for(int row = 0; row < matrixSize; row++){
    for(int col = 0; col < matrixSize; col++){
      if ( matrix[row][col] == 0) continue;
      
      int size = _nodes[row]->adjConnections(_nodes[col]).size();
      if ( matrix[row][col] != size){
        for(int i = 0; i < size; i++){
        
        }
      }
    }
  }
}

Graph::~Graph(){
//kDebug() << "Destructiong the graph";
}

Node* Graph::addNode(qreal x, qreal y){
  Node *tmpNode = new Node(x, y, this);
  emit nodeCreated(tmpNode);
  return tmpNode;
}

QList<Node*> Graph::addMultipleNodesCircular(int Number, qreal Radius, QPointF& pos){
  GraphDocument *gc = qobject_cast<GraphDocument*>(parent());
  if ( (pos.x() - Radius) < 0)  pos.setX(Radius+20); 
  if ( (pos.y() - Radius) < 0)  pos.setY(Radius+20); 
  if ( (pos.x() + Radius) > gc -> width())  pos.setX( gc->width() - Radius - 20);    
  if ( (pos.y() + Radius) > gc -> height()) pos.setY( gc->height() - Radius - 20); 

  qreal x = pos.x();
  qreal y = pos.y();

  QList<Node*> tmpList;

  const qreal delta =  (2 * M_PI) / Number;
  for (int i = 1; i <= Number; i++) {
    const qreal angle =  i * delta;    
    tmpList.append(addNode(Radius * cos(angle) + x, Radius * sin(angle) + y)); 
  }
  return tmpList;
}

QList<Node*> Graph::addMultipleNodesSquared(int Number, qreal spacing, QPointF& pos){
  int root = sqrt(Number);
   GraphDocument *gc = qobject_cast<GraphDocument*>(parent());
  qreal total = ( root * spacing);
  if ( pos.x() < 0)  pos.setX(20); 
  if ( pos.y() < 0)  pos.setY(20); 
  if ( pos.x() + total > gc -> width())  pos.setX( gc->width() - total - 20);    
  if ( pos.y() + total > gc -> height()) pos.setY( gc->height() - total - 20); 

  qreal currentx = pos.x();
  qreal currenty = pos.y();

  QList<Node*> tmpNodes;
  
  int i;
  int j;
  for(i = 0, j = 0; i < Number; i++, j++){
    tmpNodes.append(addNode(currentx, currenty));
    currentx += spacing;
    if (j == root){
      currentx = pos.x();
      currenty += spacing;
    }
  }
  return tmpNodes;
}

QList<Node*> Graph::addKGraph(int Number, qreal Radius, QPointF& pos){
  QList<Node*> tmpList = addMultipleNodesCircular(Number, Radius, pos);
    
  foreach(Node *n1, tmpList){
    foreach(Node *n2, tmpList){
      if (n1 == n2) continue;
      if (n1 -> isConnected(n2)) continue;
      n1->connect(n2);
    }
  }
  return tmpList;
}

QList<Node*> Graph::addWGraph(int Number, qreal Radius, QPointF& pos){
  QList<Node*> tmpList = addCGraph(Number-1, Radius, pos);
  Node *tmpNode = addNode(pos.x(), pos.y());

  foreach(Node *n, tmpList){
    n->connect(tmpNode);
  }
 return tmpList;
}

QList<Node*> Graph::addCGraph(int Number, qreal Radius, QPointF& pos){
   QList<Node*> tmpList = addMultipleNodesCircular(Number, Radius, pos);
   int s = tmpList.size();
   int i;

   for(i = 0; i < (s - 1); i++) {
     tmpList[i]->connect(tmpList[i+1]);
   }
   tmpList[i]->connect(tmpList[0]);
   return tmpList;
}

void Graph::toKGraph(){
  foreach(Node *n1, _nodes){
    foreach(Node *n2, _nodes){
      if (n1 == n2) continue;
      if (n1 -> isConnected(n2)) continue;
      n1->connect(n2);
    }
  }
}

void Graph::toCGraph(){
   int s = _nodes.size();
   int i;

   for(i = 0; i < (s - 1); i++) {
     _nodes[i]->connect(_nodes[i+1]);
   }
   _nodes[i]->connect(_nodes[0]);
}

void Graph::toWGraph(){
  int s = _nodes.size();
  int i;

  s -= 2;
  for(i = 0; i < s ; ++i) {
    _nodes[i]->connect(_nodes[i+1]);
  }
  _nodes[i]->connect(_nodes[0]);

  for(i = 0; i < s ; ++i){
    _nodes[i]->connect(_nodes[s-1]);
  }
}

bool Graph::isComplete(){
  int numOfEdges = _nodes.size() - 1;
  foreach(Node *n, _nodes){
    if (n->edges().size() != numOfEdges)  return false;
  }
  return true;
}

bool Graph::isCircular(){
  foreach(Node *n, _nodes){
    if (n->edges().size() != 2) return false;
  }
  return true;
}

QList< QList <int> > Graph::ajdMatrix(){
  QList< QList< int > > matrix;
  int i, j;
  int size = _nodes.size();

  for( i = 0; i < size; ++i){
	QList<int> vector;
    matrix.append( vector );
    for( j = 0; j < size; ++j){
      if  (_nodes[i]->isConnected(_nodes[j])){
        matrix[i].append( _nodes[i]->connections(_nodes[j]).size());
      }
    }
  }

  return matrix;
}

QList< QList <int> > Graph::incMatrix(){
  QList< QList< int > > matrix;
  int i, j;
  int edgesSize = _edges.size();
  int nodesSize = _nodes.size();

  for( i = 0; i < nodesSize; ++i){
	QList<int> vector;
        matrix.append( vector );

    for( j = 0; j < edgesSize; j++){
      if ((_edges[j]->to() == _nodes[i]) && (_edges[j]->from() == _nodes[i])){
        matrix[i].append(2);
      }
      else if ( ( _edges[j]->to() == _nodes[i]) || (_edges[j]->from() == _nodes[i])){
        matrix[i].append(1);
      }
      else{
        matrix[i].append(0);
      }
    }
  }
  return matrix;
}

Graph* Graph::clone(){ 
  Graph *g = new Graph( (*this) );
  return g;
}

bool Graph::checkExistence(Node *n){
  return ( _nodes.indexOf(n) == -1) ? false : true;
}

bool Graph::checkExistence(Edge *e){
  return ( _edges.indexOf(        e) == -1) ? false : true;
}

void Graph::nodeCreatedEdge(Edge *e){ 
  kDebug() << "Edge Created";
  emit edgeCreated(e) ;
}

void Graph::removeEdge(Edge* e){ 
  _edges.removeOne(e);
}

QList<Node*> Graph::nodes() { 
  return _nodes; 
} 

QList<Edge*> Graph::edges() { 
  return _edges; 
}

Node* Graph::nodeAt(int index){ 
  return _nodes.at(index); 
}

Edge* Graph::edgeAt(int index){ 
  return _edges.at(index); 
}
