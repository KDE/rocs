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
#include "libgraph_Node.h"
#include "libgraph_Edge.h"
#include "libgraph_Graph.h"

#include <QString>

using namespace libgraph;

Edge::Edge(Node *from, Node *to, Graph *parent) : QObject(parent){
  _from = from;
  _to = to;
  _from_disconnected = 0;
  _to_disconnected = 0;
}

Edge::~Edge(){
  emit removed();
  // Remove from '_from', '_to', and the Parent Graph.
  Graph *g = qobject_cast<Graph *>(parent());
  
  g   -> destroyEdge(this);  
  _to -> destroyEdge(this);
  
  if (_to != _from) _from -> destroyEdge(this);
}

bool Edge::visited() const{
  return _visited;
}

void Edge::setVisited(bool visited ){
  _visited = visited;
  emit visitedChanged(visited);
}

void Edge::unsetVisited(){
  _visited = false;
  emit visitedChanged(false);
}

QString Edge::name() const{
  return _name;
}

void Edge::setName(const QString& name){
  _name = name;
  emit nameChanged(name);
}

QColor Edge::color() const{
  return _color;
}

void Edge::setColor(QColor color){
  _color = color;
  emit colorChanged(color);
}

qreal Edge::length() const{
  return _length;
}

void Edge::setLength(const qreal& length){
  _length = length;
  emit lengthChanged(length);
}

Node* Edge::from() const{
  return _from;
}

Node* Edge::to() const{
  return _to;
}

void Edge::connectNodes(){
  if ( _to != 0) return;
  _to = _to_disconnected;
  _to_disconnected = 0;
  _from = _from_disconnected;
  _from_disconnected = 0;
}

void Edge::disconnectNodes(){
  if (_to_disconnected != 0) return;
  _to_disconnected = _to;
  _to = 0;
  _from_disconnected = _from;
  _from = 0;
}

void Edge::remove(){
  
  Graph *g = qobject_cast<Graph*>(parent());
  g->removeEdge(this);
}