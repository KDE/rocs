/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODETYPEMODEL_H
#define NODETYPEMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"

#include <QAbstractListModel>

class QSignalMapper;

namespace GraphTheory
{
class GraphDocument;
class NodeTypeModelPrivate;

class GRAPHTHEORY_EXPORT NodeTypeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GraphTheory::GraphDocumentPtr document READ document WRITE setDocument NOTIFY documentChanged)

public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1, //!< unique identifier of node type
        TitleRole, //!< title of node type
        ColorRole, //!< color of node type
        DataRole //!< access to NodeType object
    };

    explicit NodeTypeModel(QObject *parent = nullptr);
    ~NodeTypeModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const override;
    GraphDocumentPtr document() const;
    void setDocument(GraphDocumentPtr document);
    NodeTypePtr type(int index) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void nodeChanged(int index);
    void documentChanged();

private Q_SLOTS:
    void onNodeTypeAboutToBeAdded(NodeTypePtr node, int index);
    void onNodeTypeAdded();
    void onNodeTypesAboutToBeRemoved(int first, int last);
    void onNodeTypesRemoved();
    void emitNodeTypeChanged(int row);

private:
    Q_DISABLE_COPY(NodeTypeModel)
    const QScopedPointer<NodeTypeModelPrivate> d;
};
}

#endif
