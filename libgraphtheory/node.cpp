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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "node.h"
#include "nodetype.h"
#include "edge.h"
#include "nodetypestyle.h"
#include "logging_p.h"

#include <QPointF>

using namespace GraphTheory;

// initialize number of edge objects
uint Node::objectCounter = 0;

class GraphTheory::NodePrivate {
public:
    NodePrivate()
        : m_valid(false)
        , m_x(0)
        , m_y(0)
        , m_color(Qt::white)
        , m_id(-1)
    {
    }

    ~NodePrivate()
    {
    }

    NodePtr q;
    GraphDocumentPtr m_document;
    NodeTypePtr m_type;
    EdgeList m_edges;
    bool m_valid;
    qreal m_x;
    qreal m_y;
    QColor m_color;
    int m_id;
};

Node::Node()
    : QObject()
    , d(new NodePrivate)
{
    connect(this, &Node::dynamicPropertyAdded,
        this, &Node::dynamicPropertiesChanged);
    connect(this, &Node::dynamicPropertyRemoved,
        this, &Node::dynamicPropertiesChanged);

    ++Node::objectCounter;
}

Node::~Node()
{
    --Node::objectCounter;
}

NodePtr Node::create(GraphDocumentPtr document)
{
    NodePtr pi(new Node);
    pi->setQpointer(pi);
    pi->d->m_document = document;
    pi->d->m_id = document->generateId();
    pi->setType(document->nodeTypes().first());
    pi->d->m_valid = true;

    // insert completely initialized node into document
    document->insert(pi->d->q);

    return pi;
}

NodePtr Node::self() const
{
    return d->q;
}

void Node::destroy()
{
    d->m_valid = false;
    foreach (EdgePtr edge, d->m_edges) {
        d->m_document->remove(edge);
    }
    d->m_document->remove(d->q);

    // reset last reference to this object
    d->q.reset();
}

bool Node::isValid() const
{
    return d->m_valid;
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

void Node::setType(NodeTypePtr type)
{
    Q_ASSERT(d->m_document->nodeTypes().contains(type));
    if (d->m_type == type) {
        return;
    }
    if (d->m_type) {
        d->m_type->disconnect(this);
        d->m_type->style()->disconnect(this);
    }
    d->m_type = type;
    connect(type.data(), &NodeType::dynamicPropertyAboutToBeAdded,
        this, &Node::dynamicPropertyAboutToBeAdded);
    connect(type.data(), &NodeType::dynamicPropertyAdded,
        this, &Node::dynamicPropertyAdded);
    connect(type.data(), &NodeType::dynamicPropertiesAboutToBeRemoved,
        this, &Node::dynamicPropertiesAboutToBeRemoved);
    connect(type.data(), &NodeType::dynamicPropertyRemoved,
        this, &Node::dynamicPropertyRemoved);
    connect(type.data(), &NodeType::dynamicPropertyRemoved,
        this, &Node::updateDynamicProperty);
    connect(type.data(), &NodeType::dynamicPropertyRenamed,
        this, &Node::renameDynamicProperty);
    connect(type->style(), &NodeTypeStyle::changed,
        this, &Node::styleChanged);
    emit typeChanged(type);
    emit styleChanged();
}

void Node::insert(EdgePtr edge)
{
    if (edge->from() != d->q && edge->to() != d->q) {
        return;
    }
    if (d->m_edges.contains(edge)) {
        return;
    }
    d->m_edges.append(edge);
    emit edgeAdded(edge);
}

void Node::remove(EdgePtr edge)
{
    if (edge && edge->isValid()) {
        edge->destroy();
    }
    // efficient way to remove edge without having to preserve order
    for (int i = 0; i < d->m_edges.length(); ++i) {
        if (d->m_edges.at(i) == edge) {
            d->m_edges[i] = d->m_edges.last();
            d->m_edges.removeLast();
            return;
        }
    }
}

EdgeList Node::edges(EdgeTypePtr type) const
{
    if (!type) {
        return d->m_edges;
    }
    EdgeList edges;
    foreach (EdgePtr edge, d->m_edges) {
        if (edge->type() == type) {
            edges.append(edge);
        }
    }
    return edges;
}

EdgeList Node::inEdges(EdgeTypePtr type) const
{
    EdgeList inEdges;
    foreach (EdgePtr edge, d->m_edges) {
        if (type && edge->type() != type) {
            continue;
        }
        if (edge->type()->direction() == EdgeType::Bidirectional) {
            inEdges.append(edge);
            continue;
        }
        if (edge->type()->direction() == EdgeType::Unidirectional
            && edge->to() == self()
        ) {
            inEdges.append(edge);
        }
    }
    return inEdges;
}

EdgeList Node::outEdges(EdgeTypePtr type) const
{
    EdgeList outEdges;
    foreach (EdgePtr edge, d->m_edges) {
        if (type && edge->type() != type) {
            continue;
        }
        if (edge->type()->direction() == EdgeType::Bidirectional) {
            outEdges.append(edge);
            continue;
        }
        if (edge->type()->direction() == EdgeType::Unidirectional
            && edge->from() == self()
        ) {
            outEdges.append(edge);
        }
    }
    return outEdges;
}

int Node::id() const
{
    return d->m_id;
}

void Node::setId(int id)
{
    if (id == d->m_id) {
        return;
    }
    d->m_id = id;
    emit idChanged(id);
}

qreal Node::x() const
{
    return d->m_x;
}

void Node::setX(qreal x)
{
    if (x == d->m_x) {
        return;
    }
    d->m_x = x;
    emit positionChanged(QPointF(x, d->m_y));
}

qreal Node::y() const
{
    return d->m_y;
}

void Node::setY(qreal y)
{
    if (y == d->m_y) {
        return;
    }
    d->m_y = y;
    emit positionChanged(QPointF(d->m_x, y));
}

QColor Node::color() const
{
    return d->m_color;
}

void Node::setColor(const QColor &color)
{
    if (color == d->m_color) {
        return;
    }
    d->m_color = color;
    emit colorChanged(color);
}

QVariant Node::dynamicProperty(const QString &property) const
{
    return Node::property(("_graph_" + property).toLatin1());
}

QStringList Node::dynamicProperties() const
{
    if (!d->m_type) {
        return QStringList();
    }
    return d->m_type->dynamicProperties();
}

void Node::setDynamicProperty(const QString &property, const QVariant &value)
{
    if (!d->m_type) {
        qCWarning(GRAPHTHEORY_GENERAL) << "No type registered, aborting to set property.";
    }
    if (value.isValid() && !d->m_type->dynamicProperties().contains(property)) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Dynamic property not registered at type, aborting to set property.";
    }
    setProperty(("_graph_" + property).toLatin1(), value);
    emit dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(property));
}

void Node::updateDynamicProperty(const QString &property)
{
    // remove property if not registered at type
    if (!d->m_type->dynamicProperties().contains(property)) {
        setDynamicProperty(property, QVariant::Invalid);
    }
    emit dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(property));
}

void Node::renameDynamicProperty(const QString &oldProperty, const QString &newProperty)
{
    setDynamicProperty(newProperty, dynamicProperty(oldProperty));
    setDynamicProperty(oldProperty, QVariant::Invalid);
    emit dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(newProperty));
}

void Node::setQpointer(NodePtr q)
{
    d->q = q;
}
