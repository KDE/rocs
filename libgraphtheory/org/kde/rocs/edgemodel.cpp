// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#include "edgemodel.h"
#include "edgeproxy.h"
#include "graphdocument.h"
#include <QDebug>
#include <QSignalMapper>

using namespace GraphTheory;

class GraphTheory::EdgeModelPrivate
{
public:
    void updateMappings()
    {
        int edges = m_document->edges().count();
        for (int i = 0; i < edges; i++) {
            m_signalMapper.setMapping(m_document->edges().at(i).data(), i);
        }
    }

    QList<EdgeProxy *> m_edgeProxyList;
    GraphDocumentPtr m_document;
    QSignalMapper m_signalMapper;
};

EdgeModel::EdgeModel(QObject *parent)
    : QAbstractListModel(parent)
    , d(new EdgeModelPrivate)
{
    connect(&d->m_signalMapper, &QSignalMapper::mappedInt, this, &EdgeModel::emitEdgeChanged);
}

EdgeModel::~EdgeModel() = default;

QHash<int, QByteArray> EdgeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[DataRole] = "edge";

    return roles;
}

void EdgeModel::setDocument(GraphDocumentPtr document)
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
        connect(d->m_document.data(), &GraphDocument::edgeAboutToBeAdded, this, &EdgeModel::onEdgeAboutToBeAdded);
        connect(d->m_document.data(), &GraphDocument::edgeAdded, this, &EdgeModel::onEdgeAdded);
        connect(d->m_document.data(), &GraphDocument::edgesAboutToBeRemoved, this, &EdgeModel::onEdgesAboutToBeRemoved);
        connect(d->m_document.data(), &GraphDocument::edgesRemoved, this, &EdgeModel::onEdgesRemoved);
    }
    for (const auto &edge : d->m_document->edges()) {
        d->m_edgeProxyList.push_back(new EdgeProxy(edge));
    }
    endResetModel();
}

QVariant EdgeModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(d->m_document);

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= d->m_document->edges().count()) {
        return QVariant();
    }

    EdgePtr const edge = d->m_document->edges().at(index.row());

    switch (role) {
    case DataRole:
        return QVariant::fromValue<EdgeProxy *>(d->m_edgeProxyList.at(index.row()));
    default:
        return QVariant();
    }
}

int EdgeModel::rowCount(const QModelIndex &parent) const
{
    if (!d->m_document) {
        return 0;
    }

    if (parent.isValid()) {
        return 0;
    }

    return d->m_document->edges().count();
}

void EdgeModel::onEdgeAboutToBeAdded(EdgePtr edge, int index)
{
    Q_UNUSED(edge)
    d->m_edgeProxyList.insert(index, new EdgeProxy(edge));
    // TODO add missing signals
    beginInsertRows(QModelIndex(), index, index);
}

void EdgeModel::onEdgeAdded()
{
    d->updateMappings();
    endInsertRows();
}

void EdgeModel::onEdgesAboutToBeRemoved(int first, int last)
{
    beginRemoveRows(QModelIndex(), first, last);
    const auto removalStagedProxies = d->m_edgeProxyList.sliced(first, last - first);
    d->m_edgeProxyList.remove(first, last - first);
    for (const auto proxy : removalStagedProxies) {
        proxy->deleteLater();
    }
}

void EdgeModel::onEdgesRemoved()
{
    endRemoveRows();
}

void EdgeModel::emitEdgeChanged(int row)
{
    Q_EMIT edgeChanged(row);
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
}

#include "moc_edgemodel.cpp"
