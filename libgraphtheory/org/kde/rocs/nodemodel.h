// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace GraphTheory
{
class GraphDocument;
class NodeModelPrivate;

class GRAPHTHEORY_EXPORT NodeModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1, //!< unique identifier of node
        PositionRole,
        DataRole //!< access to Node object
    };

    explicit NodeModel(QObject *parent = nullptr);
    ~NodeModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const override;
    void setDocument(GraphDocumentPtr document);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

Q_SIGNALS:
    void nodeChanged(int index);

private Q_SLOTS:
    void onNodeAboutToBeAdded(GraphTheory::NodePtr node, int index);
    void onNodeAdded();
    void onNodesAboutToBeRemoved(int first, int last);
    void onNodesRemoved();
    void emitNodeChanged(int row);

private:
    Q_DISABLE_COPY(NodeModel)
    const QScopedPointer<NodeModelPrivate> d;
};
}

#endif
