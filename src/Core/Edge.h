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
#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <QColor>

class Graph;
class Node;

class Edge : public QObject{
  Q_OBJECT

  public:
    Edge(Node* from, Node* to, Graph* parent); // Default constructor
    ~Edge(); // Default destructor 
    int relativeIndex();
    bool operator<(Edge *e);
  public slots:
    
  Node* to();   // gets the To node
  Node* from(); // gets the from node

  void remove(); // remove this edge

  bool  visited()  { return _visited; }
  QString  name()  { return _name;    }
  qreal    length(){ return _length;  }
  qreal    value() { return _value;   }
  qreal    total() { return _total;   }
  QColor   color() { return _color;   }

  void  setVisited(bool v){ _visited = v; emit visitedChanged(v); }
  void  setName(QString n){ _name = n;    emit nameChanged(n);    }
  void  setLength(qreal l){ _length = l;  emit lengthChanged(l);  }
  void  setValue(qreal v) { _value = v;   emit valueChanged(v);   }
  void  setTotal(qreal t) { _total = t;   emit totalChanged(t);   }
  void  setColor(QColor c){ _color = c;   emit colorChanged(c);   }

  signals:
    void visitedChanged(bool v);
    void nameChanged(const QString& s);
    void lengthChanged(qreal l);
    void valueChanged(qreal v);
    void totalChanged(qreal t);
    void colorChanged(QColor c);
    void removed(); // emitted when the edge`s about to be removed.

  private:
    Node* _from;
    Node* _to;
    bool _visited;
    QString _name;
    qreal   _length;
    qreal   _value;
    qreal   _total;
    QColor  _color;

};

#endif
