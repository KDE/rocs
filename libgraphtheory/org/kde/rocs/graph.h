// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef GRAPH_H
#define GRAPH_H

#include "edgeproxy.h"
#include "nodeproxy.h"
#include <QObject>
#include <QQmlEngine>

namespace GraphTheory
{
class Graph : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Backend adapter that is owned by view")

public:
    Graph(QObject *parent = nullptr);
    ~Graph() override = default;
    void setGraphDocument(GraphDocumentPtr graphDocument);

public Q_SLOTS:
    void createNode(qreal x, qreal y, int typeId);
    void createEdge(GraphTheory::NodeProxy *from, GraphTheory::NodeProxy *to, int typeIndex);
    void deleteNode(GraphTheory::NodeProxy *nodeProxy);
    void deleteEdge(GraphTheory::EdgeProxy *edge);

private:
    GraphDocumentPtr m_document;
};
}

#endif
