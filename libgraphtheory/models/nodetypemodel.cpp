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

#include "nodetypemodel.h"
#include "graphdocument.h"

#include <KLocalizedString>
#include <QSignalMapper>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::NodeTypeModelPrivate {
public:
    NodeTypeModelPrivate()
        : m_signalMapper(new QSignalMapper)
    {
    }

    ~NodeTypeModelPrivate()
    {
        m_signalMapper->deleteLater();
    }

    void updateMappings()
    {
        int types = m_document->nodeTypes().count();
        for (int i = 0; i < types; ++i) {
            m_signalMapper->setMapping(m_document->nodeTypes().at(i).data(), i);
        }
    }

    GraphDocumentPtr m_document;
    QSignalMapper *m_signalMapper;
};

NodeTypeModel::NodeTypeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodeTypeModelPrivate)
{
    connect(d->m_signalMapper, SIGNAL(mapped(int)), SLOT(emitNodeTypeChanged(int)));
}

NodeTypeModel::~NodeTypeModel()
{

}

QHash< int, QByteArray > NodeTypeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DataRole] = "dataRole";

    return roles;
}

void NodeTypeModel::setDocument(GraphDocumentPtr document)
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
        connect(d->m_document.data(), SIGNAL(nodeTypeAboutToBeAdded(NodeTypePtr,int)), SLOT(onNodeTypeAboutToBeAdded(NodeTypePtr,int)));
        connect(d->m_document.data(), SIGNAL(nodeTypeAdded()), SLOT(onNodeTypeAdded()));
        connect(d->m_document.data(), SIGNAL(nodeTypesAboutToBeRemoved(int,int)), SLOT(onNodeTypesAboutToBeRemoved(int,int)));
        connect(d->m_document.data(), SIGNAL(nodeTypesRemoved()), SLOT(onNodeTypesRemoved()));
    }
    endResetModel();
}

QVariant NodeTypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    Q_ASSERT(d->m_document);

    if (index.row() >= d->m_document->nodeTypes().count()) {
        return QVariant();
    }

    NodeTypePtr const type = d->m_document->nodeTypes().at(index.row());

    switch(role)
    {
    case IdRole:
        return type->id();
    case TitleRole:
        return type->name();
    case ColorRole:
        return type->color();
    case DataRole:
        return QVariant::fromValue<QObject*>(type.data());
    default:
        return QVariant();
    }
}

bool NodeTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qWarning() << "Index not valid, aborting";
        return false;
    }
    Q_ASSERT(d->m_document);

    if (index.row() >= d->m_document->nodeTypes().count()) {
        return false;
    }

    NodeTypePtr const type = d->m_document->nodeTypes().at(index.row());
    switch(role)
    {
    case IdRole:
        type->setId(value.toInt());
        return true;
    case TitleRole:
        type->setName(value.toString());
        return true;
    case ColorRole:
        type->setColor(value.value<QColor>());
        return true;
    default:
        return false;
    }
}

int NodeTypeModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_document) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_document->nodeTypes().count();
}

void NodeTypeModel::onNodeTypeAboutToBeAdded(NodeTypePtr type, int index)
{
    beginInsertRows(QModelIndex(), index, index);
    connect(type.data(), SIGNAL(idChanged(int)), d->m_signalMapper, SLOT(map()));
    connect(type.data(), SIGNAL(nameChanged(QString)), d->m_signalMapper, SLOT(map()));
    connect(type.data(), SIGNAL(colorChanged(QColor)), d->m_signalMapper, SLOT(map()));
}

void NodeTypeModel::onNodeTypeAdded()
{
    d->updateMappings();
    endInsertRows();
}

void NodeTypeModel::onNodeTypesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void NodeTypeModel::onNodeTypesRemoved()
{
    endRemoveRows();
}

void NodeTypeModel::emitNodeTypeChanged(int row)
{
    emit nodeChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant NodeTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "Node Type"));
}
