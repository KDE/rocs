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

#include <QPointF>
#include <QColor>
#include <QString>
#include "Node.h"
#include "Edge.h"
#include "Graph.h"
#include <KDebug>

Node::Node( qreal x, qreal y, Graph *parent ) : QObject(parent){
  _color = parent->color();
  _pos = QPointF(x, y);
  _name = "";
  _total = 1;
  _value = 1;
  _visited = false;
   parent->append(this);
}
  
Node::~Node(){

}

void Node::remove(){
	
}

void Node::removeEdge(Edge* e){
  _edges.removeOne(e);	
}

bool Node::hasEdge(Edge* e){
	foreach(Edge *ed, _edges){
		if (ed == e) return true;
	}
	return false;
}

bool Node::isConnected(Node* n){
  foreach(Edge *e, _edges){
    if (( e->to() == n) || (e->from() == n)) return true;
  }
  return false;
}

QList<Node*> Node::nodes(){
  QList<Node*> tmpList;
  
  foreach(Edge *e, _edges){
    if ( e->from() == e->to() )  tmpList.append(this);
    else if( e->from() == this ) tmpList.append(e->to());
    else if( e->to() == this )   tmpList.append(e->from());
  }

  return tmpList;
}

QList<Edge*> Node::edges(Node *n){
  if ( n == 0 ){ return _edges; }
  
  QList<Edge*> tmpEdges;
  foreach(Edge *e, _edges){
    if ((e -> from() == n) || (e -> to() == n)) tmpEdges.append(e);
  }
    
  return tmpEdges;
}

Node* Node::node(){
  foreach(Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    return (e->to() == this) ? e->from() : e->to();
  }
  return 0;
}

Node* Node::unvisitedNode(){
  foreach(Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    Node *n  = (e->to() == this) ? e->from() : e->to();
    if ( ! n->visited() ) return n;
  }
  return 0;
}

Edge* Node::edge(){
  if (_edges.size() == 0) return 0;
  return _edges[0];
}

Edge* Node::unvisitedEdge(){
  if (_edges.size() == 0) return 0;
  foreach(Edge *e, _edges){
    if ( ! e->visited() ) return e;
  }
  return 0;
}

Edge* Node::connect(Node* n){
  if (n == this) return 0;
  if ( isConnected(n) ) return 0;
  
  Graph *g = qobject_cast<Graph*>(parent()); 
  if (! g->checkExistence(n) ){ return 0; }

  Edge *e = new Edge(this, n, g);
  g->nodeCreatedEdge(e);
  kDebug() << "Simple Edge Created";  
  return e;
}

void Node::append(Edge *e){
  if (_edges.indexOf(e) == -1) _edges.append(e);
}
