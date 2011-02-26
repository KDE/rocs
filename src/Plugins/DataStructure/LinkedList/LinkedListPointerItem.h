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

#ifndef LINKEDLISTPOINTERITEM_H
#define LINKEDLISTPOINTERITEM_H

#include <QGraphicsLineItem>
#include "PointerItem.h"

class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
  long explanation here...
*/

class LinkedListPointerItem : public PointerItem {
    Q_OBJECT
public:
    /*! default constructor
    \param node the libgraph::Node that this item will interact to.
    \param parent the QGraphicsITem that this Item belongs to. */
    explicit LinkedListPointerItem(Pointer *edge, QGraphicsItem *parent = 0);
    virtual ~LinkedListPointerItem();

private:
    QPainterPath createCurves();
    QPolygonF createEndArrow() const;
    QPolygonF createPath(const QPointF &pos1, const QPointF &pos2) const;
};

#endif //POINTERITEM_H
