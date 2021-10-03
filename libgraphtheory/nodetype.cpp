/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodetype.h"
#include "nodetypestyle.h"
#include "graphdocument.h"
#include <QDebug>

using namespace GraphTheory;

// initialize number of node type objects
uint NodeType::objectCounter = 0;

class GraphTheory::NodeTypePrivate {
public:
    NodeTypePrivate()
        : m_id(-1)
        , m_style(new NodeTypeStyle)
        , m_name(QString())
        , m_valid(false)
    {
        m_style->setColor(QColor(77, 77, 77)); // dark gray
    }

    ~NodeTypePrivate()
    {
        m_style->deleteLater();
    }

    NodeTypePtr q;
    int m_id;
    NodeTypeStyle *m_style;
    GraphDocumentPtr m_document;
    QStringList m_dynamicProperties;
    QString m_name;
    bool m_valid;
};

NodeType::NodeType()
    : QObject()
    , d(new NodeTypePrivate)
{
    ++NodeType::objectCounter;

    connect(d->m_style, &NodeTypeStyle::colorChanged,
        this, &NodeType::colorChanged);
}

NodeType::~NodeType()
{
    --NodeType::objectCounter;
}

NodeTypePtr NodeType::self() const
{
    return d->q;
}

NodeTypePtr NodeType::create(GraphDocumentPtr document)
{
    NodeTypePtr pi(new NodeType);
    pi->setQpointer(pi);
    pi->d->m_document = document;
    pi->d->m_id = document->generateId();
    pi->d->m_valid = true;

    // insert completely initialized node type into document
    document->insert(pi->d->q);
    return pi;
}

void NodeType::destroy()
{
    d->m_valid = false;
    d->m_document->remove(d->q);

    // reset last reference to this object
    d->q.reset();
}

bool NodeType::isValid() const
{
    return d->m_valid;
}

GraphDocumentPtr NodeType::document() const
{
    return d->m_document;
}

void NodeType::setName(const QString& name)
{
    if (d->m_name == name) {
        return;
    }
    d->m_name = name;
    Q_EMIT nameChanged(name);
}

QString NodeType::name() const
{
    return d->m_name;
}

int NodeType::id() const
{
    return d->m_id;
}

void NodeType::setId(int id)
{
    if (id == d->m_id) {
        return;
    }
    d->m_id = id;
    Q_EMIT idChanged(id);
}

NodeTypeStyle * NodeType::style() const
{
    return d->m_style;
}

QStringList NodeType::dynamicProperties() const
{
    return d->m_dynamicProperties;
}

void NodeType::addDynamicProperty(const QString& property)
{
    if (d->m_dynamicProperties.contains(property)) {
        return;
    }
    Q_EMIT dynamicPropertyAboutToBeAdded(property, d->m_dynamicProperties.count());
    d->m_dynamicProperties.append(property);
    Q_EMIT dynamicPropertyAdded();
}

void NodeType::removeDynamicProperty(const QString& property)
{
    if (!d->m_dynamicProperties.contains(property)) {
        return;
    }
    int index = d->m_dynamicProperties.indexOf(property);
    Q_EMIT dynamicPropertiesAboutToBeRemoved(index, index);
    d->m_dynamicProperties.removeAt(index);
    Q_EMIT dynamicPropertyRemoved(property);
}

void NodeType::renameDynamicProperty(const QString& oldProperty, const QString& newProperty)
{
    Q_ASSERT(d->m_dynamicProperties.contains(oldProperty));
    Q_ASSERT(!d->m_dynamicProperties.contains(newProperty));

    if (!d->m_dynamicProperties.contains(oldProperty) || d->m_dynamicProperties.contains(newProperty)) {
        return;
    }
    int index = d->m_dynamicProperties.indexOf(oldProperty);
    d->m_dynamicProperties[index] = newProperty;
    Q_EMIT dynamicPropertyRenamed(oldProperty, newProperty);
    Q_EMIT dynamicPropertyChanged(index);
}

void NodeType::setQpointer(NodeTypePtr q)
{
    d->q = q;
}
