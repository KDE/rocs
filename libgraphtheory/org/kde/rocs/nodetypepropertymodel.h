// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef NODETYPEPROPERTYMODEL_H
#define NODETYPEPROPERTYMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace GraphTheory
{
class GraphDocument;
class NodeTypePropertyModelPrivate;

class GRAPHTHEORY_EXPORT NodeTypePropertyModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GraphTheory::NodeType *node READ node WRITE setNodeType NOTIFY nodeChanged)

public:
    enum NodeTypePropertyRoles {
        NameRole = Qt::UserRole + 1, //!< unique identifier of node
    };

    explicit NodeTypePropertyModel(QObject *parent = nullptr);
    ~NodeTypePropertyModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const override;
    void setNodeType(NodeType *node);
    NodeType *node() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

Q_SIGNALS:
    void nodeChanged();
    void propertyChanged(int index);

private Q_SLOTS:
    void onDynamicPropertyAboutToBeAdded(const QString &, int index);
    void onDynamicPropertyAdded();
    void onDynamicPropertiesAboutToBeRemoved(int first, int last);
    void onDynamicPropertyRemoved();
    void onDynamicPropertyChanged(int row);

private:
    Q_DISABLE_COPY(NodeTypePropertyModel)
    const QScopedPointer<NodeTypePropertyModelPrivate> d;
};
}

#endif
