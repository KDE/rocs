/*
 *  SPDX-FileCopyrightText: 2014-2015 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodetypemodel.h"
#include "graphdocument.h"
#include "nodetypestyle.h"
#include "logging_p.h"

#include <KLocalizedString>
#include <QSignalMapper>

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
    connect(d->m_signalMapper, static_cast<void (QSignalMapper::*)(int)>(&QSignalMapper::mapped),
        this, &NodeTypeModel::emitNodeTypeChanged);
}

NodeTypeModel::~NodeTypeModel()
{

}

QHash< int, QByteArray > NodeTypeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "titleRole";
    roles[DataRole] = "dataRole";

    return roles;
}

GraphDocumentPtr NodeTypeModel::document() const
{
    return d->m_document;
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
        connect(d->m_document.data(), &GraphDocument::nodeTypeAboutToBeAdded,
            this, &NodeTypeModel::onNodeTypeAboutToBeAdded);
        connect(d->m_document.data(), &GraphDocument::nodeTypeAdded,
            this, &NodeTypeModel::onNodeTypeAdded);
        connect(d->m_document.data(), &GraphDocument::nodeTypesAboutToBeRemoved,
            this, &NodeTypeModel::onNodeTypesAboutToBeRemoved);
        connect(d->m_document.data(), &GraphDocument::nodeTypesRemoved,
            this, &NodeTypeModel::onNodeTypesRemoved);
    }
    endResetModel();
    Q_EMIT documentChanged();
}

NodeTypePtr NodeTypeModel::type(int index) const
{
    return d->m_document->nodeTypes().at(index);
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
        return type->style()->color();
    case DataRole:
        return QVariant::fromValue<QObject*>(type.data());
    default:
        return QVariant();
    }
}

bool NodeTypeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Index not valid, aborting";
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
        type->style()->setColor(value.value<QColor>());
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
    connect(type.data(), &NodeType::idChanged,
        d->m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
    connect(type.data(), &NodeType::nameChanged,
        d->m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
    connect(type.data(), &NodeType::colorChanged,
        d->m_signalMapper, static_cast<void (QSignalMapper::*)(void)>(&QSignalMapper::map));
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
    Q_EMIT nodeChanged(row);
    Q_EMIT dataChanged(index(row, 0), index(row, 0));
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
