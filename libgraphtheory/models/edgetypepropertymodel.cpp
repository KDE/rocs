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

#include "edgetypepropertymodel.h"
#include "edgetype.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::EdgeTypePropertyModelPrivate {
public:
    EdgeTypePropertyModelPrivate()
    {
    }

    ~EdgeTypePropertyModelPrivate()
    {

    }

    EdgeTypePtr m_type;
};

EdgeTypePropertyModel::EdgeTypePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new EdgeTypePropertyModelPrivate)
{

}

EdgeTypePropertyModel::~EdgeTypePropertyModel()
{

}

QHash< int, QByteArray > EdgeTypePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";

    return roles;
}

void EdgeTypePropertyModel::setEdgeType(EdgeType *type)
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
        connect(d->m_type.data(), &EdgeType::dynamicPropertyAboutToBeAdded,
            this, &EdgeTypePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_type.data(), &EdgeType::dynamicPropertyAdded,
            this, &EdgeTypePropertyModel::onDynamicPropertyAdded);
        connect(d->m_type.data(), &EdgeType::dynamicPropertiesAboutToBeRemoved,
            this, &EdgeTypePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_type.data(), &EdgeType::dynamicPropertyRemoved,
            this, &EdgeTypePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_type.data(), &EdgeType::dynamicPropertyChanged,
            this, &EdgeTypePropertyModel::onDynamicPropertyChanged);
    }
    endResetModel();
    emit edgeChanged();
}

EdgeType * EdgeTypePropertyModel::edge() const
{
    return d->m_type.data();
}

QVariant EdgeTypePropertyModel::data(const QModelIndex &index, int role) const
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

int EdgeTypePropertyModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_type) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }
    return d->m_type->dynamicProperties().count();
}

void EdgeTypePropertyModel::onDynamicPropertyAboutToBeAdded(const QString &property, int index)
{
    Q_UNUSED(property);
    beginInsertRows(QModelIndex(), index, index);
}

void EdgeTypePropertyModel::onDynamicPropertyAdded()
{
    endInsertRows();
}

void EdgeTypePropertyModel::onDynamicPropertiesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void EdgeTypePropertyModel::onDynamicPropertyRemoved()
{
    endRemoveRows();
}

void EdgeTypePropertyModel::onDynamicPropertyChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant EdgeTypePropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "EdgeTypeProperty"));
}
