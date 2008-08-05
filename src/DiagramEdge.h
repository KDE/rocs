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

#ifndef DIAGRAM_EDGE_H
#define DIAGRAM_EDGE_H

#include <QGraphicsItem>
#include <QObject>
#include <QScriptValue>

 class Node;
class QPainterPath;
class DiagramScene;

 class Edge :public QObject, public QGraphicsItem
 {
    Q_OBJECT
 public:    
    enum ArrowType{NoArrow, SingleArrow, DoubleArrow};
    enum { Type = UserType + 2 };

    Edge(Node *sourceNode,        // the `from` node
        Node *destNode,           // The `to` node
        int arrowType = 0,        // enum ArrowType
        qreal length = 0,         // the length between the 2 nodes.
        bool show_length = false, // Should the Scene display the length of this two nodes?
        DiagramScene *parent = 0
    );
    
    ~Edge();
    void adjust();
    int type() const { return Type; }
    bool selected;
    bool isShowLengthEnabled() const { return show_length; }
    bool operator==(Edge *e);
    
  public slots:
    QScriptValue getFrom();
    QScriptValue getTo();
    bool isSelected(){  return selected;  }
    qreal getLength() const { return length; }
    void setLength(double d);
    void showLength(int i);
    
 protected:
    QPainterPath shape() const;  
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

 private:
    QScriptValue getNode(int pos);
    DiagramScene *diagramScene;
    Node *source, *dest;
    int arrowType;
    QPointF sourcePoint;
    QPointF destPoint;
    QLineF *myLine;
    qreal length;
    qreal arrowSize;
    bool show_length;

 };

#endif
