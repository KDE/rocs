/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODEPROPERTYMODEL_H
#define NODEPROPERTYMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QAbstractListModel>

class QSignalMapper;

namespace GraphTheory
{
class GraphDocument;
class NodePropertyModelPrivate;

class GRAPHTHEORY_EXPORT NodePropertyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(GraphTheory::Node *node READ node WRITE setNode NOTIFY nodeChanged)

public:
    enum NodePropertyRoles {
        NameRole = Qt::UserRole + 1,      //!< unique identifier of node
        ValueRole,                        //!< access to property value
        VisibilityRole                    //!< is property name visible
    };

    explicit NodePropertyModel(QObject *parent = nullptr);
    ~NodePropertyModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int,QByteArray> roleNames() const override;
    void setNode(Node *node);
    Node * node() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void nodeChanged();
    void propertyChanged(int index);

private Q_SLOTS:
    void onDynamicPropertyAboutToBeAdded(const QString&, int index);
    void onDynamicPropertyAdded();
    void onDynamicPropertiesAboutToBeRemoved(int first, int last);
    void onDynamicPropertyRemoved();
    void onDynamicPropertyChanged(int row);

private:
    Q_DISABLE_COPY(NodePropertyModel)
    const QScopedPointer<NodePropertyModelPrivate> d;
};
}

#endif
