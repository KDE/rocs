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
#define EDGE_h

#include <QObject>
#include <QColor>

namespace libgraph
{

class Node;
class Graph;

class Edge : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool visited READ visited WRITE setVisited RESET unsetVisited)
  Q_PROPERTY(QString name READ name WRITE setName)
  Q_PROPERTY(qreal length READ length WRITE setLength)
  Q_PROPERTY(QColor color READ color WRITE setColor)


  public:
    /*! Default Constructor */
    Edge(Node *from, Node *to, Graph *parent);

    /*! Default Destructor */
    ~Edge();

    /*! gets visited flag */
    bool visited() const;

    /*! visit this edge */
    void setVisited(bool);

    /*! unvisit this edge */
    void unsetVisited();

    /*! return the name of this edge */
    QString name() const;

    /*! sets the name of this edge */
    void setName(const QString& name);
    
    /*! return the length of the edge */
    qreal length() const;

    /*! set the length of this edge */
    void setLength(const qreal& length);

    /*! gets the color of this edge */
    QColor color() const;

    /*! sets the color of this edge */
    void setColor(QColor color);

    /*! gets the from node */
    Node* from() const;

    /*! gets the to node */
    Node* to() const;
    
    /*! reconnect this edge to the nodes */
    void connectNodes();

    /*! disconnect this edge from the nodes */
    void disconnectNodes();

  signals:
    /*! emited when the name changes */
    void nameChanged(QString name);

    /*! emited when the length changed */
    void lengthChanged(qreal length);

    /*! emited when the visited status changes */
    void visitedChanged(bool visited);

    /*! emited when the color changes */
    void colorChanged(QColor color);

  private:
    Node* _from;
    Node* _to;
    Node* _from_disconnected;
    Node* _to_disconnected;
    bool _visited;
    QString _name;
    qreal _length;
    QColor _color;

};

};
#endif
