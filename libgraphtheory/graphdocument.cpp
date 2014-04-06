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

#include "graphdocument.h"
#include "edgetype.h"
#include "nodetype.h"
#include "edge.h"

#include <QDebug>

using namespace GraphTheory;

// initialize number of edge objects
uint GraphDocument::objectCounter = 0;

class GraphTheory::GraphDocumentPrivate {
public:
    GraphDocumentPrivate()
        : m_valid(false)
    {
        // create default type
        EdgeType::create(q);
        NodeType::create(q);
    }

    ~GraphDocumentPrivate()
    {
    }

    GraphDocumentPtr q;
    QList<EdgeTypePtr> m_edgeTypes;
    QList<NodeTypePtr> m_nodeTypes;
    NodeList m_nodes;
    bool m_valid;
};

void GraphDocument::destroy()
{
    d->m_valid = false;
    foreach (NodePtr node, d->m_nodes) {
        node->destroy();
    }
    d->m_nodes.clear();

    // reset last reference to this object
    d->q.reset();
}

GraphDocument::GraphDocument()
    : QObject()
    , d(new GraphDocumentPrivate)
{
    ++GraphDocument::objectCounter;
}

GraphDocument::~GraphDocument()
{

    --GraphDocument::objectCounter;
}

GraphDocumentPtr GraphDocument::create()
{
    GraphDocumentPtr pi(new GraphDocument);
    pi->setQpointer(pi);
    pi->d->m_valid = true;
    return pi;
}

NodeList GraphDocument::nodes(NodeTypePtr type) const
{
    if (!type) {
        return d->m_nodes;
    }

    NodeList nodes;
    foreach (NodePtr node, d->m_nodes) {
        if (node->type() == type) {
            nodes.append(node);
        }
    }
    return nodes;
}

EdgeList GraphDocument::edges(EdgeTypePtr type) const
{
    EdgeList edges;
    foreach (NodePtr node, d->m_nodes) {
        foreach (EdgePtr edge, node->edges()) {
            if (edge->from() != node) { // count edges only once
                continue;
            }
            if (!type) { // if no type is set, return all types
                edges.append(edge);
            }
            else if (edge->type() == type) {
                edges.append(edge);
            }
        }
    }
    return edges;
}

void GraphDocument::insert(NodePtr node)
{
    Q_ASSERT(node);
    Q_ASSERT(node->document() == d->q);

    if (!node || d->m_nodes.contains(node)) {
        return;
    }

    d->m_nodes.append(node);
}

void GraphDocument::insert(NodeTypePtr type)
{
    Q_ASSERT(type);
    if (d->m_nodeTypes.contains(type)) {
        return;
    }
    d->m_nodeTypes.append(type);
}

void GraphDocument::insert(EdgeTypePtr type)
{
    Q_ASSERT(type);
    if (d->m_edgeTypes.contains(type)) {
        return;
    }
    d->m_edgeTypes.append(type);
}

void GraphDocument::remove(NodePtr node)
{
    if (node->isValid()) {
        node->destroy();
    }
    d->m_nodes.removeOne(node);
}

void GraphDocument::remove(NodeTypePtr type)
{
    foreach (NodePtr node, d->m_nodes) {
        if (node->type() == type) {
            node->destroy();
        }
    }
    if (type->isValid()) {
        type->destroy();
    }
    d->m_nodeTypes.removeOne(type);
}

void GraphDocument::remove(EdgeTypePtr type)
{
    foreach (EdgePtr edge, edges(type)) {
        edge->destroy();
    }
    if (type->isValid()) {
        type->destroy();
    }
    d->m_edgeTypes.removeOne(type);
}

QList< EdgeTypePtr > GraphDocument::edgeTypes() const
{
    Q_ASSERT(d->m_edgeTypes.length() > 0);
    return d->m_edgeTypes;
}

QList< NodeTypePtr > GraphDocument::nodeTypes() const
{
    Q_ASSERT(d->m_nodeTypes.length() > 0);
    return d->m_nodeTypes;
}

void GraphDocument::setQpointer(GraphDocumentPtr q)
{
    d->q = q;
}
