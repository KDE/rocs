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

#include "nodemodel.h"
#include "graphdocument.h"

#include <KLocalizedString>
#include <QSignalMapper>

using namespace GraphTheory;

class GraphTheory::NodeModelPrivate {
public:
    NodeModelPrivate()
        : m_signalMapper(new QSignalMapper)
    {
    }

    ~NodeModelPrivate()
    {
        m_signalMapper->deleteLater();
    }

    void updateMappings()
    {
        int nodes = m_document->nodes().count();
        for (int i = 0; i < nodes; i++) {
            m_signalMapper->setMapping(m_document->nodes().at(i).data(), i);
        }
    }

    GraphDocumentPtr m_document;
    QSignalMapper *m_signalMapper;
};

NodeModel::NodeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodeModelPrivate)
{
    connect(d->m_signalMapper, SIGNAL(mapped(int)), SLOT(emitNodeChanged(int)));
}

NodeModel::~NodeModel()
{

}

QHash< int, QByteArray > NodeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DataRole] = "dataRole";

    return roles;
}

void NodeModel::setDocument(GraphDocumentPtr document)
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
        connect(d->m_document.data(), SIGNAL(nodeAboutToBeAdded(NodePtr,int)), SLOT(onNodeAboutToBeAdded(NodePtr,int)));
        connect(d->m_document.data(), SIGNAL(nodeAdded()), SLOT(onNodeAdded()));
        connect(d->m_document.data(), SIGNAL(nodesAboutToBeRemoved(int,int)), SLOT(onNodesAboutToBeRemoved(int,int)));
        connect(d->m_document.data(), SIGNAL(nodesRemoved()), SLOT(onNodesRemoved()));
    }
    endResetModel();
}

QVariant NodeModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_document);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_document->nodes().count()) {
        return QVariant();
    }

    NodePtr const node = d->m_document->nodes().at(index.row());

    switch(role)
    {
//     case IdRole: //TODO ids are not implemented yet
//         return node->id();
    case DataRole:
        return QVariant::fromValue<QObject*>(node.data());
    default:
        return QVariant();
    }
}

int NodeModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_document) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_document->nodes().count();
}

void NodeModel::onNodeAboutToBeAdded(NodePtr node, int index)
{
    //TODO add missing signals
    beginInsertRows(QModelIndex(), index, index);
}

void NodeModel::onNodeAdded()
{
    d->updateMappings();
    endInsertRows();
}

void NodeModel::onNodesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
}

void NodeModel::onNodesRemoved()
{
    endRemoveRows();
}

void NodeModel::emitNodeChanged(int row)
{
    emit nodeChanged(row);
    emit dataChanged(index(row, 0), index(row, 0));
}

QVariant NodeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "Node"));
}
