/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodetypepropertymodel.h"
#include "nodetype.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::NodeTypePropertyModelPrivate {
public:
    NodeTypePropertyModelPrivate()
    {
    }

    ~NodeTypePropertyModelPrivate()
    {

    }

    NodeTypePtr m_type;
};

NodeTypePropertyModel::NodeTypePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodeTypePropertyModelPrivate)
{

}

NodeTypePropertyModel::~NodeTypePropertyModel()
{

}

QHash< int, QByteArray > NodeTypePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";

    return roles;
}

void NodeTypePropertyModel::setNodeType(NodeType *type)
{
    if (d->m_type == type->self()) {
        return;
    }

    beginResetModel();
    if (d->m_type) {
        d->m_type.data()->disconnect(this);
    }
    d->m_type = type->self();
    if (d->m_type) {
        connect(d->m_type.data(), &NodeType::dynamicPropertyAboutToBeAdded,
            this, &NodeTypePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_type.data(), &NodeType::dynamicPropertyAdded,
            this, &NodeTypePropertyModel::onDynamicPropertyAdded);
        connect(d->m_type.data(), &NodeType::dynamicPropertiesAboutToBeRemoved,
            this, &NodeTypePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_type.data(), &NodeType::dynamicPropertyRemoved,
            this, &NodeTypePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_type.data(), &NodeType::dynamicPropertyChanged,
            this, &NodeTypePropertyModel::onDynamicPropertyChanged);
    }
    endResetModel();
    Q_EMIT nodeChanged();
}

NodeType * NodeTypePropertyModel::node() const
{
    return d->m_type.data();
}

QVariant NodeTypePropertyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_type);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_type->dynamicProperties().count()) {
        return QVariant();
    }

    QString const property = d->m_type->dynamicProperties().at(index.row());

    switch(role)
    {
    case NameRole:
        return property;
    default:
        return QVariant();
    }
}

int NodeTypePropertyModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_type) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }
    return d->m_type->dynamicProperties().count();
}

void NodeTypePropertyModel::onDynamicPropertyAboutToBeAdded(const QString &property, int index)
{
    Q_UNUSED(property);
    beginInsertRows(QModelIndex(), index, index);
}

void NodeTypePropertyModel::onDynamicPropertyAdded()
{
    endInsertRows();
}

void NodeTypePropertyModel::onDynamicPropertiesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void NodeTypePropertyModel::onDynamicPropertyRemoved()
{
    endRemoveRows();
}

void NodeTypePropertyModel::onDynamicPropertyChanged(int row)
{
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
}

QVariant NodeTypePropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "NodeTypeProperty"));
}
