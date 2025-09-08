// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef NODEPROXY_H
#define NODEPROXY_H

#include "node.h"
#include <QObject>
#include <QPointF>
#include <QQmlEngine>

namespace GraphTheory
{
class NodeProxy : public QObject
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Node)
    QML_UNCREATABLE("NodeProxy for Node object is managed by Graph")

    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged)

public:
    NodeProxy(NodePtr node);
    ~NodeProxy() override = default;

    NodePtr node() const;
    QPointF position() const;

Q_SIGNALS:
    void positionChanged(QPointF);

private:
    NodePtr m_node;
};
}

#endif
