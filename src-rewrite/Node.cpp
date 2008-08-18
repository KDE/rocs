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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "Node.h"
#include "Edge.h"
#include "Graph.h"

Node::Node(Graph *graph) : QObject(graph)
{
   _graph = graph;
}

void Node::addTo( Node *to )
{
   _toList.append( to );
}

void Node::addEdge( Edge *e )
{
   _edgeList.append( e );
}

QList<Edge*> Node::getConnections( Node *to )
{
  QList<Edge*> c;

  foreach(Edge *e, _edgeList){
     if ( e->isConnected(this, to) ) c.append(e);
  }

  return c;
}

void Node::setName(const QString& name)
{
  _name = name;
}

void Node::setColor(const QColor& color)
{
  _color = color;
}

void Node::setXY( qreal x, qreal y)
{
  _x = x;
  _y = y;
}