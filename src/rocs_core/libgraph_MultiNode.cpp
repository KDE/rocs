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
#include "libgraph_MultiNode.h"
#include "libgraph_Edge.h"

using namespace libgraph;

MultiNode::MultiNode(QPointF position, Graph *parent) : Node(position, parent){}

MultiNode::~MultiNode(){
  foreach(Edge *e, _edges)  delete e;
  _edges.clear();
}

bool MultiNode::isConnected(Node* to){

  foreach(Edge *e, _edges){
    if ((e->to() == to) && (e->from() == this)) return true;
    if ( (e->from() == to) && (e->to() == this)) return true;
  }

  return false;

}

void MultiNode::addEdge(Edge *e){
  _edges.append(e);
}

// Return the first node that this node is connected to. 
Node* MultiNode::getAdjacentNode(){
  if (_edges.size() == 0)  return 0;
  return getAdjacentNode( _edges[0] );
}

// Return the node connected with this node using edge e.
Node* MultiNode::getAdjacentNode(Edge *e){
  if (_edges.indexOf(e) == -1)  return 0;
  return ( e->from() == this) ? e->to() : e-> from();
}

// Return the first node that has not been visited and this node is connected to 
Node* MultiNode::getFirstUnvisitedNode(){
  foreach(Edge *e, _edges)  {
    Node *n = getAdjacentNode(e);
    if ( !n -> visited() ){
      return n;
    }
  }

  return 0;
}

// Return the first edge on the list, does not care if it's visited or not 
Edge* MultiNode::getFirstEdge(){
  if (_edges.size() == 0)  return 0;
  return _edges[0];
}

// Return the first unvisited edge on the list
Edge* MultiNode::getFirstUnvisitedEdge(){
  foreach(Edge *e, _edges){
    if ( ! e->visited() )  return e;
  }

  return 0;
}
