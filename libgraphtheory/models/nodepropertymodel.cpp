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

#include "nodepropertymodel.h"
#include "node.h"
#include "nodetypestyle.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::NodePropertyModelPrivate {
public:
    NodePropertyModelPrivate()
    {
    }

    ~NodePropertyModelPrivate()
    {

    }

    NodePtr m_node;

};

NodePropertyModel::NodePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodePropertyModelPrivate)
{

}

NodePropertyModel::~NodePropertyModel()
{

}

QHash< int, QByteArray > NodePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    roles[VisibilityRole] = "visibility";

    return roles;
}

void NodePropertyModel::setNode(Node* node)
{
    if (d->m_node == node->self()) {
        return;
    }

    beginResetModel();
    if (d->m_node) {
        d->m_node.data()->disconnect(this);
    }
    d->m_node = node->self();
    if (d->m_node) {
        connect(d->m_node.data(), &Node::dynamicPropertyAboutToBeAdded,
            this, &NodePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_node.data(), &Node::dynamicPropertyAdded,
            this, &NodePropertyModel::onDynamicPropertyAdded);
        connect(d->m_node.data(), &Node::dynamicPropertiesAboutToBeRemoved,
            this, &NodePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_node.data(), &Node::dynamicPropertyRemoved,
            this, &NodePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_node.data(), &Node::dynamicPropertyChanged,
            this, &NodePropertyModel::onDynamicPropertyChanged);
        connect(d->m_node.data(), &Node::styleChanged,[=]() {
            QVector<int> changedRoles;
            changedRoles.append(VisibilityRole);
            emit dataChanged(index(0), index(d->m_node->dynamicProperties().count() - 1), changedRoles);
        } );
    }
    endResetModel();
    emit nodeChanged();
}

Node * NodePropertyModel::node() const
{
    return d->m_node.data();
}

QVariant NodePropertyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_node);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_node->dynamicProperties().count()) {
        return QVariant();
    }

    QString const property = d->m_node->dynamicProperties().at(index.row());

    switch(role)
    {
    case NameRole:
        return property;
    case ValueRole:
        return d->m_node->dynamicProperty(property);
    case VisibilityRole:
        return d->m_node->type()->style()->isPropertyNamesVisible();
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

    return d->m_node->dynamicProperties().count();
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
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant NodePropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "NodeProperty"));
}
