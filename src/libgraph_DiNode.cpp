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
#include "libgraph_DiNode.h"
#include "libgraph_Graph.h"
#include "libgraph_Edge.h"

using namespace libgraph;

DiNode::DiNode(QPointF position, Graph *parent) 
: Node(position, parent){
}

DiNode::~DiNode(){
  foreach(Edge *e, _edges){
    delete e;
  }
  _edges.clear();
}

bool DiNode::isConnected(Node *to){
  foreach(Edge *e, _edges){ // Being connected doesn't mean that we can walk from this node to the other node.
    if ( (e -> to() == to)  || (e -> from() == to) )  return true;
  }
  return false;
}

void DiNode::addEdge(Edge *e){
  if (_edges.indexOf(e) != -1)  return; // Does not append the same edge twice.
  _edges.append(e);
}

// Return the first node that this node is connected to. 
Node *DiNode::getAdjacentNode(){ // TODO: Need to test

  if (_edges.size() == 0) return 0;

  Node *node = 0;
  foreach(Edge *e, _edges){
    node = getAdjacentNode(e);
    if (node != 0)  break;
  }

  if (node == 0) return 0;

  return node;
}

// return the node connected with this using edge e 
Node *DiNode::getAdjacentNode(Edge *e){ // TODO: Need to test
  if ( e -> from() == this )  return e->to();
  return 0;
}

// Return the first node that has not been visited and this node is connected to 
Node *DiNode::getFirstUnvisitedNode(){ // TODO: need to test
  if (_edges.size() == 0)  return 0;

  Node *node = 0;
  foreach(Edge *e, _edges){
    node = getAdjacentNode(e);
    if (node->visited() == false){
      return node;
    }
  }
  return 0;
}

// Return the first edge on the list, does not care if it's visited or not 
Edge *DiNode::getFirstEdge(){ // TODO: need to test
  if (_edges.size() == 0)  return 0;
  return _edges[0];
}

// Return the first unvisited edge on the list 
Edge *DiNode::getFirstUnvisitedEdge(){ // TODO: need to test
  foreach(Edge *e, _edges){
    if ( e->from() != this)       continue;
    if ( e -> visited() == true ) continue;
    return e;
  }
  return 0;
}
