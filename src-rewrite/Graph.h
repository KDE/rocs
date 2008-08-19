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

#ifndef GRAPH_H
#define GRAPH_H

#include <QList>
#include <QVariant>
#include <QColor>
#include <QString>

class Edge;
class Node;
class QString;

class Graph : public QObject{
Q_OBJECT
public:
  Graph(int GraphType, QObject *parent);
  enum Type{ SimpleType = QVariant::UserType + 1, MultigraphType ,DigraphType };
  
  bool isSimple();
  bool isMultigraph();
  bool isDigraph();
  bool isNull();
  bool isComplete();
  bool isCircular();
  bool isWheel();
  bool isCubic();
  
  void makeComplement();
  QList< QList<int> > incidencyMatrix();
  QList< QList<int> > adjacencyMatrix();
  
private:
  QColor  _color;   //! Color of the outlines of the Graph on the screen.
  QString _comment; //! Some Comment of the graph.
  QString _name;   //! Name of the Graph

  QList<Edge*> _edges; //! List of edges
  QList<Node*> _nodes; //! list of Nodes.


public slots:
  void setName(const QString& n);
  void addNode(qreal x, qreal y);
  void addEdge(int nodeIndex1, int nodeIndex2);
  void addEdge(Node *n1, Node *n2);
  void setColor(const QColor& c);
  
signals:
  void nodeInserted(Node *n);
  void edgeInserted(Edge *e);

};

#endif
