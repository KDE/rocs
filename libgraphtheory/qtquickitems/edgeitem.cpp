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
        , m_origin(0, 0)
        , m_pointFrom(0, 0)
        , m_pointTo(0, 0)
        , m_nodeWidth(64)
    {
    }

    ~EdgeItemPrivate()
    {
    }
    Edge *m_edge;
    QPointF m_origin;
    QPointF m_pointFrom, m_pointTo;
    const int m_nodeWidth;
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
    if (edge) {
        edge->from().data()->disconnect(this);
        edge->to().data()->disconnect(this);
        edge->disconnect(this);
    }
    d->m_edge = edge;
    connect (edge->from().data(), SIGNAL(positionChanged(QPointF)), this, SLOT(updatePosition()));
    connect (edge->to().data(), SIGNAL(positionChanged(QPointF)), this, SLOT(updatePosition()));
    updatePosition();
    emit edgeChanged();
}

QPointF EdgeItem::origin() const
{
    return d->m_origin;
}

void EdgeItem::setOrigin(const QPointF &origin)
{
    if (d->m_origin == origin) {
        return;
    }
    d->m_origin = origin;
    updatePosition();
}

void EdgeItem::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QBrush(Qt::black), 2, Qt::SolidLine));
    painter->drawLine(d->m_pointFrom, d->m_pointTo);
}

void EdgeItem::updatePosition()
{
    // compute bounding box
    // the box possibly has to be enlarged to contain at least the whole width of the line
    qreal boxXglobal = qMin(d->m_edge->from()->x(), d->m_edge->to()->x()); // global coordinate
    qreal boxYglobal = qMin(d->m_edge->from()->y(), d->m_edge->to()->y()); // global coordinate
    qreal boxWidth = qAbs(d->m_edge->to()->x() - d->m_edge->from()->x());
    qreal boxHeight = qAbs(d->m_edge->to()->y() - d->m_edge->from()->y());

    // set coordinates
    setX(boxXglobal - d->m_origin.x());
    setY(boxYglobal - d->m_origin.y());
    setWidth(boxWidth);
    setHeight(boxHeight);

    // set from/to values relative to box x/y position
    d->m_pointFrom = QPointF(d->m_edge->from()->x(), d->m_edge->from()->y())
                        - d->m_origin
                        - QPointF(x(), y());
    d->m_pointTo = QPointF(d->m_edge->to()->x(), d->m_edge->to()->y())
                        - d->m_origin
                        - QPointF(x(), y());
    update();
}
