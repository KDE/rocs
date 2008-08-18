/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "Graph.h"
#include "Edge.h"
#include "Node.h"

#include <KLocale>

Graph::Graph(int graphType, QObject *parent) : QObject(parent)
{
  setName(i18n("untitled"));
  setColor(Qt::black);
  setProperty("graph_type", graphType);
}

void Graph::setName(const QString& n)
{
  _name = n;
}

void Graph::addNode(qreal x, qreal y)
{
  Node *n = new Node(this);
  n -> setXY( x, y);
  _nodes.append(n);
  emit nodeInserted( n );
}

void Graph::addEdge(int nodeIndex1, int nodeIndex2)
{
  Node *n1 = _nodes[nodeIndex1];
  Node *n2 = _nodes[nodeIndex1];
  addEdge(n1, n2);
}

void Graph::addEdge(Node *from, Node *to)
{
   Edge *e = new Edge(this);
   e->setFrom(from);
   e->setTo(to);
   from -> addTo(to);
   if (property("graph_type") == GraphType)
   {
      to -> addTo(from);
   }
   _edges.append( e );
   emit edgeInserted( e );
}

void Graph::setColor(const QColor& c)
{
   _color = c;
}