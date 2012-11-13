/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef ROOTEDTREE_H
#define ROOTEDTREE_H

#include <QGraphicsLineItem>
#include <QPointer>
#include "Pointer.h"
#include "Scene/PointerItem.h"

class QGraphicsSceneMouseEvent;
class Pointer;
class QGraphicsSimpleTextItem;

/*! \brief the Edge drawing on screen.
  long explanation here...
*/
class RootedTreeEdgeItem : public PointerItem
{
    Q_OBJECT
    QPointF startPoint();
    QPointF endPoint() const;

public:
    /**
     * Default constructor.
     *
     * \param edge the tree edge represented by this graphics item
     * \param parent the graphics scene to that the edge belongs
     */
    explicit RootedTreeEdgeItem(PointerPtr edge, QGraphicsItem *parent = 0);
    virtual ~RootedTreeEdgeItem();

    QPainterPath createCurves();

public slots:
    void updatePathLayout();
};

#endif //ROOTEDTREE_H
