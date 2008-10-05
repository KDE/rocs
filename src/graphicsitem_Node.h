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

#ifndef GRAPHICSITEM_NODE_H
#define GRAPHICSITEM_NODE_H

#include <QGraphicsItem>
#include <QList>

class QGraphicsSceneMouseEvent;

namespace libgraph{
  class Node;
}

class NodeItem : public QGraphicsItem{

public:
  NodeItem(libgraph::Node *node, QGraphicsItem *parent = 0);

  enum { Type = UserType + 1 };
  int type() const { return Type; }

  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
  void setPos(QPointF pos);

protected:
   QVariant itemChange(GraphicsItemChange change, const QVariant &value);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
  void positionChanged(QPointF pos);

private:
    libgraph::Node *_node;
};

#endif