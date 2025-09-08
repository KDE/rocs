// SPDX-FileCopyrightText: 2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "graph.h"
#include "edge.h"
#include "graphdocument.h"
#include "nodetype.h"
#include <algorithm>

using namespace GraphTheory;

Graph::Graph(QObject *parent)
    : QObject(parent)
{
}

void Graph::setGraphDocument(GraphDocumentPtr graphDocument)
{
    m_document = graphDocument;
}

void Graph::createNode(qreal x, qreal y, int typeId)
{
    const auto types = m_document->nodeTypes();
    auto type = std::find_if(std::begin(types), std::end(types), [typeId](NodeTypePtr nodeType) {
        return typeId == nodeType->id();
    });
    if (type != types.end()) {
        NodePtr node = Node::create(m_document);
        node->setType(*type);
        node->setX(x);
        node->setY(y);
    } else {
        qCritical() << "Trying to create node with unknown node type id" << typeId << "skipping";
    }
}

void Graph::createEdge(NodeProxy *from, NodeProxy *to, int typeId)
{
    if (!from || !to) {
        return;
    }
    const auto types = m_document->edgeTypes();
    auto type = std::find_if(std::begin(types), std::end(types), [typeId](EdgeTypePtr edgeType) {
        return typeId == edgeType->id();
    });
    if (type == types.end()) {
        qCritical() << "Trying to create edge with unknown edge type id" << typeId << "skipping";
        return;
    }
    if (!from->node()->isValid() || !to->node()->isValid()) {
        return;
    }
    EdgePtr edge = Edge::create(from->node()->self(), to->node()->self());
    edge->setType(*type);
}

void Graph::deleteNode(GraphTheory::NodeProxy *proxy)
{
    if (!proxy || !proxy->node() || !proxy->node()->isValid()) {
        return;
    }
    proxy->node()->destroy();
}

void Graph::deleteEdge(GraphTheory::EdgeProxy *proxy)
{
    if (!proxy || !proxy->edge() || !proxy->edge()->isValid()) {
        return;
    }
    proxy->edge()->destroy();
}
