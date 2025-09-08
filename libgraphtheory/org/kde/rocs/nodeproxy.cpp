// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "nodeproxy.h"
#include <QPointF>

using namespace GraphTheory;

NodeProxy::NodeProxy(NodePtr node)
    : QObject(node.get())
    , m_node(node)
{
    Q_ASSERT(node);
    if (node) {
        connect(m_node.get(), &Node::positionChanged, this, &NodeProxy::positionChanged);
    }
}

NodePtr NodeProxy::node() const
{
    return m_node;
}

QPointF NodeProxy::position() const
{
    return m_node == nullptr ? QPointF(m_node->x(), m_node->y()) : QPointF();
}
