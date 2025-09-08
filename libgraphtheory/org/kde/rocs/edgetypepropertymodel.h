// SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef EDGETYPEPROPERTYMODEL_H
#define EDGETYPEPROPERTYMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace GraphTheory
{
class GraphDocument;
class EdgeTypePropertyModelPrivate;

class GRAPHTHEORY_EXPORT EdgeTypePropertyModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GraphTheory::EdgeType *edge READ edge WRITE setEdgeType NOTIFY edgeChanged)

public:
    enum EdgeTypePropertyRoles {
        NameRole = Qt::UserRole + 1, //!< unique identifier of edge
    };

    explicit EdgeTypePropertyModel(QObject *parent = nullptr);
    ~EdgeTypePropertyModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const override;
    void setEdgeType(EdgeType *edge);
    EdgeType *edge() const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

Q_SIGNALS:
    void edgeChanged();
    void propertyChanged(int index);

private Q_SLOTS:
    void onDynamicPropertyAboutToBeAdded(const QString &, int index);
    void onDynamicPropertyAdded();
    void onDynamicPropertiesAboutToBeRemoved(int first, int last);
    void onDynamicPropertyRemoved();
    void onDynamicPropertyChanged(int row);

private:
    Q_DISABLE_COPY(EdgeTypePropertyModel)
    const QScopedPointer<EdgeTypePropertyModelPrivate> d;
};
}

#endif
