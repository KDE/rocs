
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


#include "OrientedNode.h"
#include "Edge.h"
#include "OrientedGraph.h"
#include <KDebug>


bool OrientedNode::isAdjacent(Node* n){  
 foreach(Edge *e, _edges){
    if ( e->to() == n) return true;
  }
  return false;
}

QList<Node*> OrientedNode::adjNodes(){
  QList<Node*> tmpList;
  foreach(Edge *e, _edges){
    if (e->from() == e->to()) tmpList.append(this);
    if (e->to() != this) tmpList.append(e->to());
  }
  return tmpList;
}

Node* OrientedNode::adjNode(){
  foreach( Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    if ( e->to() != this) return e->to();
  }
  return 0;
}

Node* OrientedNode::unvisitedAdjNode(){
  foreach( Edge *e, _edges){
    if ( e->from() == e->to()) return this;
    if (( e->to() != this) && ( ! e->to()->visited() )) return e->to();
  }
  return 0;
}

QList<Edge*> OrientedNode::adjEdges(Node *n){
  QList<Edge*> tmpEdges;
  if (n == 0) foreach(Edge *e, _edges){
    if ( e->from() == e->to()) tmpEdges.append(e);
    if ( e->from() == this) tmpEdges.append(e);
  }
  else foreach(Edge *e, _edges){
    if ( e->to() == n) tmpEdges.append(e);
  }
  qSort(tmpEdges.begin(), tmpEdges.end());
  return tmpEdges;
}

Edge* OrientedNode::unvisitedAdjEdge(){
  if (_edges.size() == 0) return 0;
  foreach(Edge *e, _edges){
    if (( e->from() == this) && ( ! e->visited())) return e;
  }
  return 0;
}

Edge* OrientedNode::adjEdge(){
  foreach(Edge *e, _edges){
    if (e->from() == this) return e;
  }
  return 0;
}

Edge* OrientedNode::connect(Node* n){
  Graph *g = qobject_cast<Graph*>(parent()); 
  if (! g->checkExistence(n) ) return 0;
  Edge *e = 0;
  e = new Edge(this, n, g);
  g->nodeCreatedEdge(e);
  kDebug() << "Oriented Edge Created";
  return e;
}
