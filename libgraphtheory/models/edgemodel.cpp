/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edgemodel.h"
#include "edge.h"
#include "graphdocument.h"
#include <KLocalizedString>
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
    roles[DataRole] = "dataRole";

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
        return QVariant::fromValue<QObject *>(edge.data());
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

QVariant EdgeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    if (orientation == Qt::Vertical) {
        return QVariant(section + 1);
    }
    return QVariant(i18nc("@title:column", "Edge"));
}

#include "moc_edgemodel.cpp"
