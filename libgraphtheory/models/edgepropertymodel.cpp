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

#include "edgepropertymodel.h"
#include "edge.h"
#include "edgetypestyle.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::EdgePropertyModelPrivate {
public:
    EdgePropertyModelPrivate()
    {
    }

    ~EdgePropertyModelPrivate()
    {

    }

    EdgePtr m_edge;

};

EdgePropertyModel::EdgePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new EdgePropertyModelPrivate)
{

}

EdgePropertyModel::~EdgePropertyModel()
{

}

QHash< int, QByteArray > EdgePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    roles[VisibilityRole] = "visibility";

    return roles;
}

void EdgePropertyModel::setEdge(Edge *edge)
{
    if (d->m_edge == edge->self()) {
        return;
    }

    beginResetModel();
    if (d->m_edge) {
        d->m_edge.data()->disconnect(this);
    }
    d->m_edge = edge->self();
    if (d->m_edge) {
        connect(d->m_edge.data(), &Edge::dynamicPropertyAboutToBeAdded,
            this, &EdgePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_edge.data(), &Edge::dynamicPropertyAdded,
            this, &EdgePropertyModel::onDynamicPropertyAdded);
        connect(d->m_edge.data(), &Edge::dynamicPropertiesAboutToBeRemoved,
            this, &EdgePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_edge.data(), &Edge::dynamicPropertyRemoved,
            this, &EdgePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_edge.data(), &Edge::dynamicPropertyChanged,
            this, &EdgePropertyModel::onDynamicPropertyChanged);
        connect(d->m_edge.data(), &Edge::styleChanged,[=]() {
            QVector<int> changedRoles;
            changedRoles.append(VisibilityRole);
            emit dataChanged(index(0), index(d->m_edge->dynamicProperties().count() - 1), changedRoles);
        } );
    }
    endResetModel();
    emit edgeChanged();
}

Edge * EdgePropertyModel::edge() const
{
    return d->m_edge.data();
}

QVariant EdgePropertyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_edge);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_edge->dynamicProperties().count()) {
        return QVariant();
    }

    QString const property = d->m_edge->dynamicProperties().at(index.row());

    switch(role)
    {
    case NameRole:
        return property;
    case ValueRole:
        return d->m_edge->dynamicProperty(property);
    case VisibilityRole:
        return d->m_edge->type()->style()->isPropertyNamesVisible();
    default:
        return QVariant();
    }
}

int EdgePropertyModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_edge) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_edge->dynamicProperties().count();
}

void EdgePropertyModel::onDynamicPropertyAboutToBeAdded(const QString &property, int index)
{
    Q_UNUSED(property);
    beginInsertRows(QModelIndex(), index, index);
}

void EdgePropertyModel::onDynamicPropertyAdded()
{
    endInsertRows();
}

void EdgePropertyModel::onDynamicPropertiesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void EdgePropertyModel::onDynamicPropertyRemoved()
{
    endRemoveRows();
}

void EdgePropertyModel::onDynamicPropertyChanged(int row)
{
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant EdgePropertyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "EdgeProperty"));
}
