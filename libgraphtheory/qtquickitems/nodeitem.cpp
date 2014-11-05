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

#include "nodeitem.h"

#include <QPainter>
#include <qmath.h>

using namespace GraphTheory;

class GraphTheory::NodeItemPrivate {
public:
    NodeItemPrivate()
        : m_node(0)
        , m_origin(0,0)
        , m_highlighted(false)
        , m_updating(false)
    {
    }

    ~NodeItemPrivate()
    {
    }

    Node *m_node;
    QPointF m_origin;
    bool m_highlighted;
    bool m_updating; //!< while true do not react to any change requested
};

NodeItem::NodeItem(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
    , d(new NodeItemPrivate)
{
    setFlag(QQuickItem::ItemHasContents, true);

    setWidth(32);
    setHeight(32);
}

NodeItem::~NodeItem()
{

}

Node * NodeItem::node() const
{
    return d->m_node;
}

void NodeItem::setNode(Node *node)
{
    if (node == d->m_node) {
        return;
    }
    if (node) {
        node->disconnect(this);
    }
    d->m_node = node;
    setGlobalPosition(QPointF(node->x(), node->y()));
    connect(node, &Node::positionChanged,
        this, &NodeItem::setGlobalPosition);
    connect(node, SIGNAL(typeColorChanged(QColor)),
        this, SLOT(update()));
    connect(node, SIGNAL(colorChanged(QColor)),
        this, SLOT(update()));
    connect(this, &NodeItem::xChanged,
        this, &NodeItem::updatePositionfromScene);
    connect(this, &NodeItem::yChanged,
        this, &NodeItem::updatePositionfromScene);
    emit nodeChanged();
    update();
}

QPointF NodeItem::origin() const
{
    return d->m_origin;
}

void NodeItem::setOrigin(const QPointF &origin)
{
    if (d->m_origin == origin) {
        return;
    }
    // update position with new origin
    d->m_origin = origin;
    setGlobalPosition(QPointF(d->m_node->x(), d->m_node->y()));
    update();
}

bool NodeItem::isHighlighted() const
{
    return d->m_highlighted;
}

void NodeItem::setHighlighted(bool highlight)
{
    if (d->m_highlighted == highlight) {
        return;
    }
    d->m_highlighted = highlight;
    emit highlightedChanged();
    update();
}

void NodeItem::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    if (d->m_highlighted) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(246, 116, 0, 125)); // beware orange, half transparent
        painter->drawEllipse(QRectF(0, 0, width(), height()));
    }
    painter->setPen(QPen(QColor(d->m_node->type()->color()), 2, Qt::SolidLine));
    painter->setBrush(QBrush(d->m_node->color()));
    painter->drawEllipse(QRectF(4, 4, width() - 8, height() - 8));
}

bool NodeItem::contains(const QPointF &point) const
{
    // test for round objects
    QPointF center(x() + width()/2, y() + height()/2);
    QPointF distance = point - center;
    if (qSqrt(distance.x()*distance.x() + distance.y() * distance.y()) < width()/2) {
        return true;
    }
    return false;
}

void NodeItem::updatePositionfromScene()
{
    if (d->m_node->x() == x() + d->m_origin.x()
        && d->m_node->y() == y() + d->m_origin.y()
    ) {
        return;
    }
    d->m_updating = true;
    d->m_node->setX(x() + d->m_origin.x() + width()/2);
    d->m_node->setY(y() + d->m_origin.y() + height()/2);
    d->m_updating = false;
}

void NodeItem::setGlobalPosition(const QPointF &position)
{
    if (d->m_updating) {
        return;
    }
    setX(position.x() - d->m_origin.x() - width()/2);
    setY(position.y() - d->m_origin.y() - height()/2);
    update();
}