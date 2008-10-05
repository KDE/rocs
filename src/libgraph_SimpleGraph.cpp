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
#include "libgraph_SimpleGraph.h"
#include "libgraph_SimpleNode.h"
#include "libgraph_Edge.h"
#include "libgraph_GraphDocument.h"

using namespace libgraph;

SimpleGraph::SimpleGraph(GraphDocument *parent) : Graph(parent){}

SimpleGraph::~SimpleGraph(){}

Edge* SimpleGraph::createEdge(Node* from, Node* to){
  if ( _nodes.indexOf(from) == -1 ) return 0;
  if ( _nodes.indexOf(to) == -1)    return 0;
  if ( from->isConnected(to) )      return 0;
  
  Edge *e = new Edge(from, to, this);

  from->addEdge(e);
  to->addEdge(e);

  _edges.append(e);
  emit edgeCreated(e);
  return e;
}

Node* SimpleGraph::createNode(QPointF position){
  Node *n = new SimpleNode(position, this);
  _nodes.append(n);
  n -> setIndex( _nodes.size() - 1 );

  emit nodeCreated(n);
  return n;
}
