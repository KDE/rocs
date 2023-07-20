/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "node.h"
#include "edge.h"
#include "logging_p.h"
#include "nodetype.h"
#include "nodetypestyle.h"

#include <QPointF>

using namespace GraphTheory;

// initialize number of edge objects
uint Node::objectCounter = 0;

class GraphTheory::NodePrivate
{
public:
    NodePrivate()
        : m_valid(false)
        , m_x(0)
        , m_y(0)
        , m_color(Qt::white)
        , m_id(-1)
    {
    }

    ~NodePrivate() = default;

    NodePtr q;
    GraphDocumentPtr m_document;
    NodeTypePtr m_type;
    EdgeList m_edges;
    bool m_valid;
    QStringList m_dynamicPropertiesModel; //!< provides the access public list for ListItemModel
    QMap<QString, QVariant> m_dynamicProperties;
    qreal m_x;
    qreal m_y;
    QColor m_color;
    int m_id;
};

Node::Node()
    : QObject()
    , d(new NodePrivate)
{
    connect(this, &Node::dynamicPropertyAdded, this, &Node::dynamicPropertiesChanged);
    connect(this, &Node::dynamicPropertyRemoved, this, &Node::dynamicPropertiesChanged);

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
    const auto edges = d->m_edges;
    for (const EdgePtr &edge : edges) {
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
    connect(type.data(), &NodeType::dynamicPropertyAboutToBeAdded, this, &Node::dynamicPropertyAboutToBeAdded);
    connect(type.data(), &NodeType::dynamicPropertyAdded, this, &Node::dynamicPropertyAdded);
    connect(type.data(), &NodeType::dynamicPropertiesAboutToBeRemoved, this, &Node::dynamicPropertiesAboutToBeRemoved);
    connect(type.data(), &NodeType::dynamicPropertyRemoved, this, &Node::dynamicPropertyRemoved);
    connect(type.data(), &NodeType::dynamicPropertyRemoved, this, &Node::updateDynamicProperty);
    connect(type.data(), &NodeType::dynamicPropertyRenamed, this, &Node::renameDynamicProperty);
    connect(type->style(), &NodeTypeStyle::changed, this, &Node::styleChanged);
    Q_EMIT typeChanged(type);
    Q_EMIT styleChanged();
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
    Q_EMIT edgeAdded(edge);
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
    for (const EdgePtr &edge : std::as_const(d->m_edges)) {
        if (edge->type() == type) {
            edges.append(edge);
        }
    }
    return edges;
}

EdgeList Node::inEdges(EdgeTypePtr type) const
{
    EdgeList inEdges;
    for (const EdgePtr &edge : std::as_const(d->m_edges)) {
        if (type && edge->type() != type) {
            continue;
        }
        if (edge->type()->direction() == EdgeType::Bidirectional) {
            inEdges.append(edge);
            continue;
        }
        if (edge->type()->direction() == EdgeType::Unidirectional && edge->to() == self()) {
            inEdges.append(edge);
        }
    }
    return inEdges;
}

EdgeList Node::outEdges(EdgeTypePtr type) const
{
    EdgeList outEdges;
    for (const EdgePtr &edge : std::as_const(d->m_edges)) {
        if (type && edge->type() != type) {
            continue;
        }
        if (edge->type()->direction() == EdgeType::Bidirectional) {
            outEdges.append(edge);
            continue;
        }
        if (edge->type()->direction() == EdgeType::Unidirectional && edge->from() == self()) {
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
    Q_EMIT idChanged(id);
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
    Q_EMIT positionChanged(QPointF(x, d->m_y));
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
    Q_EMIT positionChanged(QPointF(d->m_x, y));
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
    Q_EMIT colorChanged(color);
}

QVariant Node::dynamicProperty(const QString &property) const
{
    return d->m_dynamicProperties.value(property);
}

QStringList Node::dynamicProperties() const
{
    return d->m_dynamicPropertiesModel;
}

void Node::setDynamicProperty(const QString &property, const QVariant &value)
{
    if (value.isValid() && !d->m_type->dynamicProperties().contains(property)) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Dynamic property not registered at type, aborting to set property.";
    }
    if (d->m_dynamicPropertiesModel.contains(property)) {
        Q_ASSERT(d->m_dynamicProperties.contains(property));
        if (value != QVariant::Invalid) {
            d->m_dynamicProperties.insert(property, value);
            Q_EMIT dynamicPropertyChanged(d->m_dynamicPropertiesModel.indexOf(property));
        } else {
            int index = d->m_dynamicPropertiesModel.indexOf(property);
            Q_EMIT dynamicPropertiesAboutToBeRemoved(index, index);
            d->m_dynamicProperties.remove(property);
            d->m_dynamicPropertiesModel.removeAt(index);
            Q_EMIT dynamicPropertyRemoved();
        }
    } else {
        Q_ASSERT(!d->m_dynamicProperties.contains(property));
        Q_EMIT dynamicPropertyAboutToBeAdded(property, d->m_dynamicPropertiesModel.size());
        d->m_dynamicProperties.insert(property, value);
        d->m_dynamicPropertiesModel << property;
        Q_EMIT dynamicPropertyAdded();
    }
}

void Node::updateDynamicProperty(const QString &property)
{
    // remove property if not registered at type
    if (!d->m_type->dynamicProperties().contains(property)) {
        setDynamicProperty(property, QVariant::Invalid);
    }
    Q_EMIT dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(property));
}

void Node::renameDynamicProperty(const QString &oldProperty, const QString &newProperty)
{
    setDynamicProperty(newProperty, dynamicProperty(oldProperty));
    setDynamicProperty(oldProperty, QVariant::Invalid);
}

void Node::setQpointer(NodePtr q)
{
    d->q = q;
}
