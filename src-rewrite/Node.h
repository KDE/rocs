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

#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QString>
#include <QColor>

class Edge;
class Graph;

class Node : public QObject{
Q_OBJECT

public:
  Node(Graph *graph);
  void addTo( Node *to );
  void addEdge( Edge *e );
  QList<Edge*> getConnections( Node *to );
  QList<Edge*> getEdges();
  bool isBegin(){ return _begin; }
  bool isEnd() { return _end; }
public slots:
  void setName(const QString& name);
  void setColor(const QColor& color);
  void setXY( qreal x, qreal y);

private:
  Graph *_graph;
  QString _name;
  int _index;
  qreal _x;
  qreal _y;
  QColor _color;
  QList<Node*> _connections;
  QList<Edge*> _edges;
  bool _begin;
  bool _end;

};
#endif
