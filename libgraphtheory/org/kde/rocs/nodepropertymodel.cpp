// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "nodepropertymodel.h"
#include "node.h"
#include "nodetypestyle.h"
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::NodePropertyModelPrivate
{
public:
    NodeProxy *m_node{nullptr};
};

NodePropertyModel::NodePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodePropertyModelPrivate)
{
}

NodePropertyModel::~NodePropertyModel() = default;

QHash<int, QByteArray> NodePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    roles[VisibilityRole] = "visibility";

    return roles;
}

void NodePropertyModel::setNode(NodeProxy *node)
{
    if (!node) {
        qWarning() << "Abort setting of null node in property model";
        return;
    }
    if (d->m_node == node) {
        return;
    }

    beginResetModel();
    if (d->m_node) {
        d->m_node->node().get()->disconnect(this);
    }
    d->m_node = node;
    if (d->m_node) {
        connect(d->m_node->node().get(), &Node::dynamicPropertyAboutToBeAdded, this, &NodePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_node->node().get(), &Node::dynamicPropertyAdded, this, &NodePropertyModel::onDynamicPropertyAdded);
        connect(d->m_node->node().get(), &Node::dynamicPropertiesAboutToBeRemoved, this, &NodePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_node->node().get(), &Node::dynamicPropertyRemoved, this, &NodePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_node->node().get(), &Node::dynamicPropertyChanged, this, &NodePropertyModel::onDynamicPropertyChanged);
        connect(d->m_node->node().get(), &Node::styleChanged, [=]() {
            QVector<int> changedRoles;
            changedRoles.append(VisibilityRole);
            Q_EMIT dataChanged(index(0), index(d->m_node->node()->dynamicProperties().count() - 1), changedRoles);
        });
    }
    endResetModel();
    Q_EMIT nodeChanged();
}

NodeProxy *NodePropertyModel::node() const
{
    return d->m_node;
}

QVariant NodePropertyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_node);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_node->node()->dynamicProperties().count()) {
        return QVariant();
    }

    QString const property = d->m_node->node()->dynamicProperties().at(index.row());

    switch (role) {
    case NameRole:
        return property;
    case ValueRole:
        return d->m_node->node()->dynamicProperty(property);
    case VisibilityRole:
        return d->m_node->node()->type()->style()->isPropertyNamesVisible();
    default:
        return QVariant();
    }
}

int NodePropertyModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_node) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_node->node()->dynamicProperties().count();
}

void NodePropertyModel::onDynamicPropertyAboutToBeAdded(const QString &property, int index)
{
    Q_UNUSED(property);
    beginInsertRows(QModelIndex(), index, index);
}

void NodePropertyModel::onDynamicPropertyAdded()
{
    endInsertRows();
}

void NodePropertyModel::onDynamicPropertiesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void NodePropertyModel::onDynamicPropertyRemoved()
{
    endRemoveRows();
}

void NodePropertyModel::onDynamicPropertyChanged(int row)
{
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
}

#include "moc_nodepropertymodel.cpp"
