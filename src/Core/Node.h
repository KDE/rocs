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
#ifndef NODE_H
#define NODE_H

#include <QObject>
#include <QString>
#include <QPointF>
#include <QColor>

#include "Edge.h"
#include "Graph.h"

class Node : public QObject
{
Q_OBJECT

public:
  Node(qreal x, qreal y, Graph *parent);
  ~Node();
  void removeEdge(Edge *e);
public slots:

  bool isConnected(Node* n);
  
  QList<Node*> nodes();

  Node* node();
  Node* unvisitedNode();

  Edge* edge();
  Edge* unvisitedEdge();
  
  virtual Edge* connect(Node* n);
  bool hasEdge(Edge* e);

  QList<Edge*> edges(Node *n = 0);
  QList<Edge*> connections(Node *n = 0){ return edges(n); }

  bool isTerminal() { return _edges.size() == 1; }
  bool isIsolated() { return !_edges.size(); }

  QPointF pos() { return _pos;      }
  QColor color(){ return _color;    }
  QString name(){ return _name;     }
  qreal value() { return _value;    }
  qreal total() { return _total;    }
  bool visited(){ return _visited;  }

  void setPos(QPointF p) { _pos = p;     emit posChanged(p);     }
  void setColor(QColor c){ _color = c;   emit colorChanged(c);   }
  void setName(QString n){ _name = n;    emit nameChanged(n);    }
  void setValue(qreal v) { _value = v;   emit valueChanged(v);   }
  void setTotal(qreal t) { _total = t;   emit totalChanged(t);   }
  void setVisited(bool v){ _visited = v; emit visitedChanged(v); }

  virtual QList<Edge*> adjEdges(Node *n = 0) { return edges(n); }
  virtual QList<Edge*> adjConnections(Node *n)    { return edges(n); }

  virtual bool isAdjacent(Node* n) { return isConnected(n);  }
  virtual QList<Node*> adjNodes()  { return nodes();         }
  virtual Node* adjNode()          { return node();          }
  virtual Node* unvisitedAdjNode() { return unvisitedNode(); }
  virtual Edge* adjEdge()          { return edge();          }
  virtual Edge* unvisitedAdjEdge() { return unvisitedEdge(); }

  void remove();
signals:
  void posChanged(QPointF pos);
  void colorChanged(QColor c);
  void nameChanged(const QString& s);
  void valueChanged(qreal v);
  void totalChanged(qreal t);
  void visitedChanged(bool v);

  void removed();
  void edgeCreated(Edge *e);

protected:
  QList<Edge*> _edges;
  QColor _color;
  QPointF _pos;
  QString _name;
  qreal _total;
  qreal _value;
  bool _visited;
  
};

#endif

