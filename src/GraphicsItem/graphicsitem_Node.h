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
class Node;

/*! \brief the node drawing on screen. 
  long explanation here...
*/
class NodeItem : public QObject, public QGraphicsItem{
Q_OBJECT
public:
  /*! default constructor 
  \param node the  Node that this item will interact to.
  \param parent the QGraphicsITem that this Item belongs to. */
 explicit NodeItem( Node *node, QGraphicsItem *parent = 0);

  /*! The type of the item */
  enum { Type = UserType + 1 };

  /*! gets the type of the item 
    \return the type of the item
  */
  int type() const { return Type; }

  /*! calculates the boundingRect of this item 
    \return the calculated boudingRect.
  */
  QRectF boundingRect() const;

  /*! calculates the shape of the node 
    \return the calculated shape.
  */
  QPainterPath shape() const;

  /*! paint the node onscreen 
    \param painter the active painter.
    \param option the QStyleOptionGraphicsItem flags
    \param widget the widget that will be affected.*/
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

   Node* node() { return _node; }

protected slots:
  void updatePos(QPointF);
  void updateName(const QString& name);
  void updateVisited(bool v);
  void updateValue(qreal v);
  void updateTotal(qreal t);
  void updateColor(QColor c);

  /*! Emited when a node is removed */
  void removed();

private:
     Node *_node;
};

#endif
