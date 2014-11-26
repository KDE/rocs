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
#include "edgetypestyle.h"
#include "qsglinenode.h"
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
        , m_nodeWidth(32)
        , m_colorDirty(false)
        , m_directionDirty(false)
        , m_visible(true)
    {
    }

    ~EdgeItemPrivate()
    {
    }
    Edge *m_edge;
    QPointF m_origin;
    QPointF m_pointFrom, m_pointTo;
    const int m_nodeWidth;
    bool m_colorDirty;
    bool m_directionDirty;
    bool m_visible;
};

EdgeItem::EdgeItem(QQuickItem *parent)
    : QQuickItem(parent)
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
    d->m_visible = edge->type()->style()->isVisible();
    connect(edge->from().data(), &Node::positionChanged,
        this, &EdgeItem::updatePosition);
    connect(edge->to().data(), &Node::positionChanged,
        this, &EdgeItem::updatePosition);
    connect(edge, &Edge::typeChanged,
        this, [&](EdgeTypePtr) { update(); });
    connect(edge, &Edge::typeColorChanged,
        this, &EdgeItem::updateColor);
    connect(edge, &Edge::directionChanged,
        this, &EdgeItem::updateDirection);
    connect(edge, &Edge::typeChanged,
        this, &EdgeItem::updateVisibility);
    connect(edge, &Edge::typeVisibilityChanged,
        this, &EdgeItem::updateVisibility);

    updatePosition();
    updateVisibility();
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

QSGNode * EdgeItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGLineNode *n = static_cast<QSGLineNode *>(node);
    if (!n) {
        n = new QSGLineNode();
        n->setDirection(d->m_edge->type()->direction());
        n->setColor(d->m_edge->type()->style()->color());
    }
    if (d->m_colorDirty) {
        n->setColor(d->m_edge->type()->style()->color());
        d->m_colorDirty = false;
    }
    if (d->m_directionDirty) {
        n->setDirection(d->m_edge->type()->direction());
        d->m_directionDirty = false;
    }

    n->setLine(d->m_pointFrom, d->m_pointTo);
    return n;
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

void EdgeItem::updateColor()
{
    d->m_colorDirty = true;
    update();
}

void EdgeItem::updateDirection()
{
    d->m_directionDirty = true;
    update();
}

void EdgeItem::updateVisibility()
{
    d->m_visible = d->m_edge->type()->style()->isVisible();
    if (d->m_visible) {
        setOpacity(1);
    } else {
        setOpacity(0);
    }
}
