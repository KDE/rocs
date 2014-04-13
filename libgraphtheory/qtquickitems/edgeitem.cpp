/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "edgeitem.h"

#include <QSGSimpleRectNode>
#include <QPainter>
#include <QPointF>

using namespace GraphTheory;

class GraphTheory::EdgeItemPrivate {
public:
    EdgeItemPrivate()
        : m_edge(0)
    {
    }

    ~EdgeItemPrivate()
    {
    }

    Edge *m_edge;
};

EdgeItem::EdgeItem(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , d(new EdgeItemPrivate)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

EdgeItem::~EdgeItem()
{

}

Edge * EdgeItem::edge() const
{
    return d->m_edge;
}

void EdgeItem::setEdge(Edge *edge)
{
    if (edge == d->m_edge) {
        return;
    }
    d->m_edge = edge;

    //TODO update when nodes are moved
    setX(qMin(edge->from()->x(), edge->to()->x()) + 32.0);
    setY(qMin(edge->from()->y(), edge->to()->y()) + 32.0);
    setWidth(qAbs(edge->to()->x() - edge->from()->x()) + 32.0);
    setHeight(qAbs(edge->to()->y() - edge->from()->y()) + 32.0);

    emit edgeChanged();
}

void EdgeItem::paint(QPainter *painter)
{
    painter->setPen(QPen(QBrush(Qt::black), 4, Qt::SolidLine));
    painter->drawLine(QPointF(0, 0), QPointF(width(), height()));
}
