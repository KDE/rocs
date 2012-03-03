/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsLineItem>
#include <QPointer>
#include <Pointer.h>
#include "PointerItem.h"

class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
   long explanation here...
 */
class EdgeItem : public PointerItem
{
    Q_OBJECT
public:
    /*! default constructor
       \param node the libgraph::Node that this item will interact to.
       \param parent the QGraphicsITem that this Item belongs to. */
    explicit EdgeItem(PointerPtr edge, QGraphicsItem *parent = 0);
    virtual ~EdgeItem();

private:
    bool _loop;
    QPainterPath createCurves();
    QPainterPath createLoop(QPointF pos) const;
    QPolygonF createArrow(const QPointF& Pos1, const QPointF& Pos2) const;

public slots:
    void updatePathLayout();
};

#endif
