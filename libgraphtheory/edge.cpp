/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edge.h"
#include "edgetypestyle.h"
#include "logging_p.h"
#include <QVariant>

using namespace GraphTheory;

// initialize number of edge objects
uint Edge::objectCounter = 0;

class GraphTheory::EdgePrivate {
public:
    EdgePrivate()
        : m_valid(false)
    {
    }

    ~EdgePrivate()
    {
    }

    EdgePtr q;
    NodePtr m_from;
    NodePtr m_to;
    EdgeTypePtr m_type;
    bool m_valid;
};

Edge::Edge()
    : QObject()
    , d(new EdgePrivate)
{
    ++Edge::objectCounter;
}

Edge::~Edge()
{
    --Edge::objectCounter;
}

EdgePtr Edge::create(NodePtr from, NodePtr to)
{
    Q_ASSERT(from);
    Q_ASSERT(to);
    Q_ASSERT(from->document() == to->document());
    EdgePtr pi(new Edge);
    pi->setQpointer(pi);
    pi->d->m_from = from;
    pi->d->m_to = to;
    pi->setType(from->document()->edgeTypes().first());

    // insert completely initialized edge into nodes' connections
    to->insert(pi->d->q);
    from->insert(pi->d->q);
    to->document()->insert(pi->d->q);
    pi->d->m_valid = true;

    return pi;
}

EdgePtr Edge::self() const
{
    return d->q;
}

void Edge::destroy()
{
    d->m_valid = false;
    d->m_from->remove(d->q);
    d->m_to->remove(d->q);
    d->m_to->document()->remove(d->q);

    // reset last reference to this object
    d->q.reset();
}

bool Edge::isValid() const
{
    return d->m_valid;
}

NodePtr Edge::from() const
{
    return d->m_from;
}

NodePtr Edge::to() const
{
    return d->m_to;
}

EdgeTypePtr Edge::type() const
{
    Q_ASSERT(d->m_type);
    return d->m_type;
}

void Edge::setType(EdgeTypePtr type)
{
    Q_ASSERT(d->m_from->document()->edgeTypes().contains(type));
    if (d->m_type == type) {
        return;
    }
    if (d->m_type) {
        d->m_type->disconnect(this);
        d->m_type->style()->disconnect(this);
    }
    d->m_type = type;
    connect(type.data(), &EdgeType::dynamicPropertyAboutToBeAdded,
        this, &Edge::dynamicPropertyAboutToBeAdded);
    connect(type.data(), &EdgeType::dynamicPropertyAdded,
        this, &Edge::dynamicPropertyAdded);
    connect(type.data(), &EdgeType::dynamicPropertiesAboutToBeRemoved,
        this, &Edge::dynamicPropertiesAboutToBeRemoved);
    connect(type.data(), &EdgeType::dynamicPropertyRemoved,
        this, &Edge::dynamicPropertyRemoved);
    connect(type.data(), &EdgeType::dynamicPropertyRemoved,
        this, &Edge::updateDynamicProperty);
    connect(type.data(), &EdgeType::directionChanged,
        this, &Edge::directionChanged);
    connect(type.data(), &EdgeType::dynamicPropertyRenamed,
        this, &Edge::renameDynamicProperty);
    connect(type->style(), &EdgeTypeStyle::changed,
        this, &Edge::styleChanged);

    Q_EMIT typeChanged(type);
    Q_EMIT styleChanged();
}

QVariant Edge::dynamicProperty(const QString &property) const
{
    return Edge::property(("_graph_" + property).toLatin1());
}

QStringList Edge::dynamicProperties() const
{
    return d->m_type->dynamicProperties();
}

void Edge::setDynamicProperty(const QString &property, const QVariant &value)
{
    if (!d->m_type) {
        qCWarning(GRAPHTHEORY_GENERAL) << "No type registered, aborting to set property.";
    }
    if (value.isValid() && !d->m_type->dynamicProperties().contains(property)) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Dynamic property not registered at type, aborting to set property.";
    }
    setProperty(("_graph_" + property).toLatin1(), value);
    Q_EMIT dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(property));
}

void Edge::updateDynamicProperty(const QString &property)
{
    // remove property if not registered at type
    if (!d->m_type->dynamicProperties().contains(property)) {
        setDynamicProperty(property, QVariant::Invalid);
    }

    Q_EMIT dynamicPropertiesChanged();
}

void Edge::renameDynamicProperty(const QString &oldProperty, const QString &newProperty)
{
    setDynamicProperty(newProperty, dynamicProperty(oldProperty));
    setDynamicProperty(oldProperty, QVariant::Invalid);
    Q_EMIT dynamicPropertyChanged(d->m_type->dynamicProperties().indexOf(newProperty));
}

void Edge::setQpointer(EdgePtr q)
{
    d->q = q;
}
