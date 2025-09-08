// SPDX-FileCopyrightText: 2014-2025 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef EDGEPROPERTYMODEL_H
#define EDGEPROPERTYMODEL_H

#include "edgeproxy.h"
#include "graphtheory_export.h"
#include <QAbstractListModel>
#include <QQmlEngine>

namespace GraphTheory
{
class GraphDocument;
class EdgePropertyModelPrivate;

class GRAPHTHEORY_EXPORT EdgePropertyModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GraphTheory::EdgeProxy *edge READ edge WRITE setEdge NOTIFY edgeChanged)

public:
    enum EdgePropertyRoles {
        NameRole = Qt::UserRole + 1, //!< unique identifier of edge
        ValueRole, //!< access to property value
        VisibilityRole //!< is property name visible
    };

    explicit EdgePropertyModel(QObject *parent = nullptr);
    ~EdgePropertyModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int, QByteArray> roleNames() const override;
    void setEdge(EdgeProxy *edge);
    EdgeProxy *edge() const;
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
    Q_DISABLE_COPY(EdgePropertyModel)
    const QScopedPointer<EdgePropertyModelPrivate> d;
};
}

#endif
