// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "nodemodel.h"
#include "graphdocument.h"
#include "nodeproxy.h"
#include <QSignalMapper>

using namespace GraphTheory;

class GraphTheory::NodeModelPrivate
{
public:
    void updateMappings()
    {
        int nodes = m_document->nodes().count();
        for (int i = 0; i < nodes; i++) {
            m_signalMapper.setMapping(m_document->nodes().at(i).data(), i);
        }
    }

    QList<NodeProxy *> m_nodeProxyList;
    GraphDocumentPtr m_document;
    QSignalMapper m_signalMapper;
};

NodeModel::NodeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new NodeModelPrivate)
{
    connect(&d->m_signalMapper, &QSignalMapper::mappedInt, this, &NodeModel::emitNodeChanged);
}

NodeModel::~NodeModel() = default;

QHash<int, QByteArray> NodeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[PositionRole] = "position";
    roles[DataRole] = "node";

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
        connect(d->m_document.data(), &GraphDocument::nodeAboutToBeAdded, this, &NodeModel::onNodeAboutToBeAdded);
        connect(d->m_document.data(), &GraphDocument::nodeAdded, this, &NodeModel::onNodeAdded);
        connect(d->m_document.data(), &GraphDocument::nodesAboutToBeRemoved, this, &NodeModel::onNodesAboutToBeRemoved);
        connect(d->m_document.data(), &GraphDocument::nodesRemoved, this, &NodeModel::onNodesRemoved);
    }
    for (const auto &node : d->m_document->nodes()) {
        d->m_nodeProxyList.push_back(new NodeProxy(node));
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

    NodeProxy *proxy = d->m_nodeProxyList.at(index.row());

    switch (role) {
    case IdRole:
        return proxy->node()->id();
    case PositionRole:
        return QPointF(proxy->node()->x(), proxy->node()->y());
    case DataRole:
        return QVariant::fromValue<NodeProxy *>(proxy);
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
    Q_UNUSED(node)
    d->m_nodeProxyList.insert(index, new NodeProxy(node));
    // TODO add missing signals
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
    const auto removalStagedProxies = d->m_nodeProxyList.sliced(first, last - first);
    d->m_nodeProxyList.remove(first, last - first);
    for (const auto proxy : removalStagedProxies) {
        proxy->deleteLater();
    }
}

void NodeModel::onNodesRemoved()
{
    endRemoveRows();
}

void NodeModel::emitNodeChanged(int row)
{
    Q_EMIT nodeChanged(row);
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
}

#include "moc_nodemodel.cpp"
