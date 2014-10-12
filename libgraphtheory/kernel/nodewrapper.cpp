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

#include "nodewrapper.h"
#include "edgewrapper.h"
#include "graphdocument.h"
#include "nodetype.h"
#include "edge.h"
#include "typenames.h"

#include <QPointF>
#include <QColor>
#include <QDebug>

using namespace GraphTheory;

NodeWrapper::NodeWrapper(NodePtr node, DocumentWrapper *documentWrapper)
    : m_node(node)
    , m_documentWrapper(documentWrapper)
{
    connect(m_node.data(), &Node::idChanged, this, &NodeWrapper::idChanged);
    connect(m_node.data(), &Node::colorChanged, this, &NodeWrapper::colorChanged);
    connect(m_node.data(), &Node::positionChanged, this, &NodeWrapper::positionChanged);
    connect(m_node.data(), &Node::dynamicPropertiesChanged, this, &NodeWrapper::updateDynamicProperties);
    connect(m_node.data(), &Node::typeChanged, this, &NodeWrapper::typeChanged);

    updateDynamicProperties();
}

NodeWrapper::~NodeWrapper()
{

}

int NodeWrapper::id() const
{
    return m_node->id();
}

qreal NodeWrapper::x() const
{
    return m_node->x();
}

void NodeWrapper::setX(qreal x)
{
    if (x == NodeWrapper::x()) {
        return;
    }
    m_node->setX(x);
    // change signal will be emitted by connection to m_node signal
}

qreal NodeWrapper::y() const
{
    return m_node->y();
}

void NodeWrapper::setY(qreal y)
{
    if (y == NodeWrapper::y()) {
        return;
    }
    m_node->setY(y);
    // change signal will be emitted by connection to m_node signal
}

QColor NodeWrapper::color() const
{
    return m_node->color();
}

void NodeWrapper::setColor(const QColor &color)
{
    if (color == m_node->color()) {
        return;
    }
    m_node->setColor(color);
    // change signal will be emitted by connection to m_node signal
}

int NodeWrapper::type() const
{
    return m_node->type()->id();
}

void NodeWrapper::setType(int typeId)
{
    NodeTypePtr newType = m_node->type();
    if (newType->id() == typeId) {
        return;
    }
    foreach (NodeTypePtr type, m_node->document()->nodeTypes()) {
        if (type->id() == typeId) {
            newType = type;
            break;
        }
    }
    if (newType == m_node->type()) {
        qWarning() << "No type with ID" << typeId << "found, aborting type change.";
        return;
    }
    m_node->setType(newType);
    // change signal will be emitted by connection to m_node signal
}

QList<GraphTheory::EdgeWrapper*> NodeWrapper::edges() const
{
    QList<EdgeWrapper*> edges;
    foreach (EdgePtr edge, m_node->edges()) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper*> NodeWrapper::inEdges() const
{
    QList<EdgeWrapper*> edges;
    foreach (EdgePtr edge, m_node->inEdges()) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper*> NodeWrapper::outEdges() const
{
    QList<EdgeWrapper*> edges;
    foreach (EdgePtr edge, m_node->outEdges()) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

bool NodeWrapper::event(QEvent* e)
{
    if (e->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent *>(e);
        QString name = QString::fromUtf8(propertyEvent->propertyName());
        QVariant value = property(propertyEvent->propertyName());
        m_node->setDynamicProperty(name, value);
    }
    return parent()->event(e);
}

void NodeWrapper::updateDynamicProperties()
{
    // set dynamic properties
    foreach (QString property, m_node->dynamicProperties()) {
        setProperty(property.toUtf8(), m_node->dynamicProperty(property));
    }
}
