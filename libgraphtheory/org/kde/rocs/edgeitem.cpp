// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "edgeitem.h"
#include "edgetypestyle.h"
#include "nodetypestyle.h"
#include "qsglinenode.h"
#include <QPainter>
#include <QPointF>
#include <QSGSimpleRectNode>

using namespace GraphTheory;

class GraphTheory::EdgeItemPrivate
{
public:
    EdgeItemPrivate()
        : m_edge(nullptr)
        , m_origin(0, 0)
        , m_pointFrom(0, 0)
        , m_pointTo(0, 0)
        , m_nodeWidth(32)
        , m_colorDirty(false)
        , m_directionDirty(false)
        , m_visible(true)
    {
    }

    EdgeProxy *m_edge{nullptr};
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

EdgeItem::~EdgeItem() = default;

EdgeProxy *EdgeItem::edge() const
{
    return d->m_edge;
}

void EdgeItem::setEdge(EdgeProxy *edge)
{
    if (!edge || !edge->edge()) {
        return;
    }
    if (d->m_edge == edge) {
        return;
    }
    if (d->m_edge) {
        d->m_edge->edge()->from().get()->disconnect(this);
        d->m_edge->edge()->to().get()->disconnect(this);
        d->m_edge->disconnect(this);
    }
    d->m_edge = edge;
    d->m_visible = edge->edge()->type()->style()->isVisible();
    connect(edge->edge()->from().get(), &Node::positionChanged, this, &EdgeItem::updatePosition);
    connect(edge->edge()->to().get(), &Node::positionChanged, this, &EdgeItem::updatePosition);
    connect(edge->edge().get(), &Edge::typeChanged, this, [&](EdgeTypePtr) {
        update();
    });
    connect(edge->edge().get(), &Edge::styleChanged, this, &EdgeItem::updateColor);
    connect(edge->edge().get(), &Edge::directionChanged, this, &EdgeItem::updateDirection);

    connect(edge->edge().get(), &Edge::styleChanged, this, &EdgeItem::updateVisibility);
    connect(edge->edge()->from().get(), &Node::styleChanged, this, &EdgeItem::updateVisibility);
    connect(edge->edge()->to().get(), &Node::styleChanged, this, &EdgeItem::updateVisibility);

    updatePosition();
    updateVisibility();
    Q_EMIT edgeChanged();
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
    Q_EMIT originChanged();
}

QSGNode *EdgeItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGLineNode *n = static_cast<QSGLineNode *>(node);
    if (!n) {
        n = new QSGLineNode();
        n->setDirection(d->m_edge->edge()->type()->direction());
        n->setColor(d->m_edge->edge()->type()->style()->color());
    }
    if (d->m_colorDirty) {
        n->setColor(d->m_edge->edge()->type()->style()->color());
        d->m_colorDirty = false;
    }
    if (d->m_directionDirty) {
        n->setDirection(d->m_edge->edge()->type()->direction());
        d->m_directionDirty = false;
    }

    n->setLine(d->m_pointFrom, d->m_pointTo);
    return n;
}

void EdgeItem::updatePosition()
{
    // compute bounding box
    // the box possibly has to be enlarged to contain at least the whole width of the line
    qreal boxXglobal = qMin(d->m_edge->edge()->from()->x(), d->m_edge->edge()->to()->x()); // global coordinate
    qreal boxYglobal = qMin(d->m_edge->edge()->from()->y(), d->m_edge->edge()->to()->y()); // global coordinate
    qreal boxWidth = qAbs(d->m_edge->edge()->to()->x() - d->m_edge->edge()->from()->x());
    qreal boxHeight = qAbs(d->m_edge->edge()->to()->y() - d->m_edge->edge()->from()->y());

    // set coordinates
    setX(boxXglobal - d->m_origin.x());
    setY(boxYglobal - d->m_origin.y());
    setWidth(boxWidth);
    setHeight(boxHeight);

    // set from/to values relative to box x/y position
    d->m_pointFrom = QPointF(d->m_edge->edge()->from()->x(), d->m_edge->edge()->from()->y()) - d->m_origin - QPointF(x(), y());
    d->m_pointTo = QPointF(d->m_edge->edge()->to()->x(), d->m_edge->edge()->to()->y()) - d->m_origin - QPointF(x(), y());
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
    bool visible = d->m_edge->edge()->type()->style()->isVisible() && d->m_edge->edge()->from()->type()->style()->isVisible()
        && d->m_edge->edge()->to()->type()->style()->isVisible();
    d->m_visible = visible;
    if (d->m_visible) {
        setOpacity(1);
    } else {
        setOpacity(0);
    }
}

#include "moc_edgeitem.cpp"
