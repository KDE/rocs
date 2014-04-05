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

#include "node.h"
#include "graphdocument.h"
#include "nodetype.h"

using namespace GraphTheory;

class GraphTheory::NodePrivate {
public:
    NodePrivate()
    {
    }

    ~NodePrivate()
    {
    }

    NodePtr q;
    GraphDocumentPtr m_document;
    NodeTypePtr m_type;
};

Node::Node()
    : QObject()
    , d(new NodePrivate)
{
}

Node::~Node()
{
}

NodePtr Node::create(GraphDocumentPtr document)
{
    NodePtr pi(new Node);
    pi->setQpointer(pi);
    pi->d->m_document = document;
    pi->d->m_type = document->nodeTypes().first();
    return pi;
}

GraphDocumentPtr Node::document() const
{
    return d->m_document;
}

NodeTypePtr Node::type() const
{
    Q_ASSERT(d->m_type);
    return d->m_type;
}

void Node::setQpointer(NodePtr q)
{
    d->q = q;
}
