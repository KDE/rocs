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

#ifndef ORIENTEDNODE_H
#define ORIENTEDNODE_H

#include "Node.h"

class Edge;
class Graph;

class OrientedNode : public Node{
Q_OBJECT
public:
  OrientedNode(qreal x, qreal y, Graph *parent) : Node(x,y,parent){}

public slots:
  bool isAdjacent(Node* n);
  QList<Edge*> adjEdges(Node *n);
  QList<Node*> adjNodes();
  Node* adjNode();
  Node* unvisitedAdjNode();
  Edge* adjEdge();
  Edge* unvisitedAdjEdge();
  Edge* connect(Node *n);
};

#endif
