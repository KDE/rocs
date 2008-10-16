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

#ifndef GRAPHICSITEM_EDGE_H
#define GRAPHICSITEM_EDGE_H

#include <QGraphicsLineItem>
#include <QList>

class QGraphicsSceneMouseEvent;

namespace libgraph{
  class Edge;
  class Node;
}
/*! \brief the Edge drawing on screen. 
  long explanation here...
*/
class EdgeItem : public QObject, public QGraphicsLineItem{
Q_OBJECT
public:
  /*! default constructor 
  \param node the libgraph::Node that this item will interact to.
  \param parent the QGraphicsITem that this Item belongs to. */
  EdgeItem(libgraph::Edge *edge, QGraphicsItem *parent = 0);

  /*! The type of the item */
  enum { Type = UserType + 2 };

  /*! gets the type of the item 
    \return the type of the item
  */
  int type() const { return Type; }

  /*! Gets the edge of the node */
  libgraph::Edge* edge() { return _edge; }
protected:
  /*! when there's a mouse click on the node, this method is invocked 
    \param event the mouse object
  */
  void mousePressEvent(QGraphicsSceneMouseEvent *event);

  /*! when the mouse is released, this method is invocked 
    \param event the mouse object
  */
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected slots:
  /*! emited when the name changes */
  void changeName(QString name);

  /*! emited when the length changed */
  void changeLength(qreal length);

  /*! emited when the visited status changes */
  void changeVisited(bool visited);

  /*! emited when the color changes */
  void changeColor(QColor color);

  /*! redraws the edge on screen, ( calls the update method() ) */
  void update2();

  /*! Emited when a node is removed */
  void removed();

private:
    libgraph::Edge *_edge;
};

#endif