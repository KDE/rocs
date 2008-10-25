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

#include "Edge.h"
#include "Graph.h"
#include "Node.h"
#include <KDebug>

Edge::Edge(Node *from, Node *to, Graph *parent) : QObject(parent){
  _from = from;
  _to = to;
  _color = parent->color();
  _visited = false;
  _name = "";
  _length = 0;
  _value = 0;
  _total = 0;
  parent->append(this);
  _from->append(this);
  _to->append(this);
}

Edge::~Edge(){
  _from->removeEdge(this);
  _to->removeEdge(this);
  Graph *g = qobject_cast<Graph*>(parent());
  g->removeEdge(this);
}

void Edge::remove(){
  delete this;
}

Node* Edge::to(){ return _to;    }
Node* Edge::from(){ return _from; }

int Edge::relativeIndex(){
  QList<Edge*> e = _from -> edges(_to);
  kDebug() << "Index dessa aresta em relacao ao no2: " << e.indexOf(this);

  return e.indexOf(this);
}

bool Edge::operator<(Edge *e){
  if ( this < e ) return true;
  return false;
}