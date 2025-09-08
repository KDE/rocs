// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "nodeitem.h"
#include "nodetypestyle.h"
#include <QPainter>

using namespace GraphTheory;

class GraphTheory::NodeItemPrivate
{
public:
    NodeItemPrivate()
        : m_node(nullptr)
        , m_origin(0, 0)
        , m_visible(true)
        , m_highlighted(false)
        , m_updating(false)
    {
    }

    ~NodeItemPrivate() = default;

    NodeProxy *m_node;
    QPointF m_origin;
    bool m_visible;
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

NodeItem::~NodeItem() = default;

NodeProxy *NodeItem::node() const
{
    return d->m_node;
}

void NodeItem::setNode(NodeProxy *proxy)
{
    if (proxy == nullptr || proxy->node() == nullptr) {
        qCritical() << "Skip setting of node==nullptr";
        return;
    }
    if (d->m_node == proxy) {
        return;
    }
    auto node = proxy->node().get();
    if (d->m_node) {
        d->m_node->disconnect(this);
    }
    d->m_node = proxy;
    setGlobalPosition(QPointF(node->x(), node->y()));
    connect(node, &Node::positionChanged, this, &NodeItem::setGlobalPosition);
    connect(node, &Node::styleChanged, this, [&]() {
        update();
    });
    connect(node, &Node::colorChanged, this, [&](QColor) {
        update();
    });
    connect(node, &Node::typeChanged, this, [&](NodeTypePtr) {
        update();
    });
    connect(node, &Node::styleChanged, this, &NodeItem::updateVisibility);
    connect(this, &NodeItem::xChanged, this, &NodeItem::updatePositionfromScene);
    connect(this, &NodeItem::yChanged, this, &NodeItem::updatePositionfromScene);

    Q_EMIT nodeChanged();
    updateVisibility();
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
    setGlobalPosition(QPointF(d->m_node->node()->x(), d->m_node->node()->y()));
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
    Q_EMIT highlightedChanged();
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
    if (d->m_node) {
        painter->setPen(QPen(QColor(d->m_node->node()->type()->style()->color()), 2, Qt::SolidLine));
        painter->setBrush(QBrush(d->m_node->node()->color()));
    }
    painter->drawEllipse(QRectF(4, 4, width() - 8, height() - 8));
}

bool NodeItem::contains(const QPointF &point) const
{
    // test for round objects
    QPointF center(x() + width() / 2, y() + height() / 2);
    QPointF distance = point - center;
    if (qSqrt(distance.x() * distance.x() + distance.y() * distance.y()) < width() / 2) {
        return true;
    }
    return false;
}

void NodeItem::updatePositionfromScene()
{
    if (d->m_node->node()->x() == x() + d->m_origin.x() && d->m_node->node()->y() == y() + d->m_origin.y()) {
        return;
    }
    d->m_updating = true;
    d->m_node->node()->setX(x() + d->m_origin.x() + width() / 2);
    d->m_node->node()->setY(y() + d->m_origin.y() + height() / 2);
    d->m_updating = false;
}

void NodeItem::setGlobalPosition(const QPointF &position)
{
    if (d->m_updating) {
        return;
    }
    setX(position.x() - d->m_origin.x() - width() / 2);
    setY(position.y() - d->m_origin.y() - height() / 2);
    update();
}

void NodeItem::updateVisibility()
{
    if (d->m_visible == d->m_node->node()->type()->style()->isVisible()) {
        return;
    }
    d->m_visible = d->m_node->node()->type()->style()->isVisible();
    if (d->m_visible) {
        setOpacity(1);
    } else {
        setOpacity(0);
    }
}

#include "moc_nodeitem.cpp"
