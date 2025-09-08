// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "edgepropertymodel.h"
#include "edge.h"
#include "edgetypestyle.h"
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::EdgePropertyModelPrivate
{
public:
    EdgeProxy *m_edge{nullptr};
};

EdgePropertyModel::EdgePropertyModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new EdgePropertyModelPrivate)
{
}

EdgePropertyModel::~EdgePropertyModel() = default;

QHash<int, QByteArray> EdgePropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    roles[VisibilityRole] = "visibility";

    return roles;
}

void EdgePropertyModel::setEdge(EdgeProxy *edge)
{
    if (!edge || !edge->edge() || d->m_edge == edge) {
        return;
    }

    beginResetModel();
    if (d->m_edge) {
        d->m_edge->edge().get()->disconnect(this);
    }
    d->m_edge = edge;
    if (d->m_edge) {
        connect(d->m_edge->edge().get(), &Edge::dynamicPropertyAboutToBeAdded, this, &EdgePropertyModel::onDynamicPropertyAboutToBeAdded);
        connect(d->m_edge->edge().get(), &Edge::dynamicPropertyAdded, this, &EdgePropertyModel::onDynamicPropertyAdded);
        connect(d->m_edge->edge().get(), &Edge::dynamicPropertiesAboutToBeRemoved, this, &EdgePropertyModel::onDynamicPropertiesAboutToBeRemoved);
        connect(d->m_edge->edge().get(), &Edge::dynamicPropertyRemoved, this, &EdgePropertyModel::onDynamicPropertyRemoved);
        connect(d->m_edge->edge().get(), &Edge::dynamicPropertyChanged, this, &EdgePropertyModel::onDynamicPropertyChanged);
        connect(d->m_edge->edge().get(), &Edge::styleChanged, this, [=]() {
            QVector<int> changedRoles;
            changedRoles.append(VisibilityRole);
            Q_EMIT dataChanged(index(0), index(d->m_edge->edge()->dynamicProperties().count() - 1), changedRoles);
        });
    }
    endResetModel();
    Q_EMIT edgeChanged();
}

EdgeProxy *EdgePropertyModel::edge() const
{
    return d->m_edge;
}

QVariant EdgePropertyModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_edge);

    if (!index.isValid() || !d->m_edge->edge()) {
        return QVariant();
    }

    if (index.row() >= d->m_edge->edge()->dynamicProperties().count()) {
        return QVariant();
    }

    QString const property = d->m_edge->edge()->dynamicProperties().at(index.row());

    switch (role) {
    case NameRole:
        return property;
    case ValueRole:
        return d->m_edge->edge()->dynamicProperty(property);
    case VisibilityRole:
        return d->m_edge->edge()->type()->style()->isPropertyNamesVisible();
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

    return d->m_edge->edge()->dynamicProperties().count();
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
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
}

#include "moc_edgepropertymodel.cpp"
