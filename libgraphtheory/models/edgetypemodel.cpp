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

#include "edgetypemodel.h"
#include "graphdocument.h"

#include <KLocalizedString>
#include <QSignalMapper>

using namespace GraphTheory;

class GraphTheory::EdgeTypeModelPrivate {
public:
    EdgeTypeModelPrivate()
        : m_signalMapper(new QSignalMapper)
    {
    }

    ~EdgeTypeModelPrivate()
    {
        m_signalMapper->deleteLater();
    }

    void updateMappings()
    {
        int types = m_document->edgeTypes().count();
        for (int i = 0; i < types; ++i) {
            m_signalMapper->setMapping(m_document->edgeTypes().at(i).data(), i);
        }
    }

    GraphDocumentPtr m_document;
    QSignalMapper *m_signalMapper;
};

EdgeTypeModel::EdgeTypeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new EdgeTypeModelPrivate)
{
    connect(d->m_signalMapper, SIGNAL(mapped(int)), SLOT(emitEdgeTypeChanged(int)));
}

EdgeTypeModel::~EdgeTypeModel()
{

}

QHash< int, QByteArray > EdgeTypeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DataRole] = "dataRole";

    return roles;
}

void EdgeTypeModel::setDocument(GraphDocumentPtr document)
{
    if (d->m_document == document) {
        return;
    }

    beginResetModel();
    if (d->m_document) {
        d->m_document.data()->disconnect(this);
    }
    d->m_document = document;
    if (d->m_document) {
        connect(d->m_document.data(), SIGNAL(edgeTypeAboutToBeAdded(EdgePtr,int)), SLOT(onEdgeTypeAboutToBeAdded(EdgePtr,int)));
        connect(d->m_document.data(), SIGNAL(edgeTypeAdded()), SLOT(onEdgeTypeAdded()));
        connect(d->m_document.data(), SIGNAL(edgeTypesAboutToBeRemoved(int,int)), SLOT(onEdgeTypesAboutToBeRemoved(int,int)));
        connect(d->m_document.data(), SIGNAL(edgeTypesRemoved()), SLOT(onEdgeTypesRemoved()));
    }
    endResetModel();
}

QVariant EdgeTypeModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_document);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_document->edgeTypes().count()) {
        return QVariant();
    }

    EdgeTypePtr const type = d->m_document->edgeTypes().at(index.row());

    switch(role)
    {
    case IdRole:
        return type->id();
    case DataRole:
        return QVariant::fromValue<QObject*>(type.data());
    default:
        return QVariant();
    }
}

int EdgeTypeModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_document) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_document->edgeTypes().count();
}

void EdgeTypeModel::onEdgeTypeAboutToBeAdded(EdgeTypePtr type, int index)
{
    //TODO add missing signals
    beginInsertRows(QModelIndex(), index, index);
}

void EdgeTypeModel::onEdgeTypeAdded()
{
    d->updateMappings();
    endInsertRows();
}

void EdgeTypeModel::onEdgeTypesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void EdgeTypeModel::onEdgeTypesRemoved()
{
    endRemoveRows();
}

void EdgeTypeModel::emitEdgeTypeChanged(int row)
{
    emit edgeChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant EdgeTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "Edge Type"));
}
