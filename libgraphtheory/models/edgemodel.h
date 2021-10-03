/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGEMODEL_H
#define EDGEMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"

#include <QAbstractListModel>

class QSignalMapper;

namespace GraphTheory
{
class GraphDocument;
class EdgeModelPrivate;

class GRAPHTHEORY_EXPORT EdgeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EdgeRoles {
        IdRole = Qt::UserRole + 1,      //!< unique identifier of node
        DataRole                        //!< access to Edge object
    };

    explicit EdgeModel(QObject *parent = nullptr);
    ~EdgeModel() override;
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    QHash<int,QByteArray> roleNames() const override;
    void setDocument(GraphDocumentPtr document);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const  override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void edgeChanged(int index);

private Q_SLOTS:
    void onEdgeAboutToBeAdded(EdgePtr node, int index);
    void onEdgeAdded();
    void onEdgesAboutToBeRemoved(int first, int last);
    void onEdgesRemoved();
    void emitEdgeChanged(int row);

private:
    Q_DISABLE_COPY(EdgeModel)
    const QScopedPointer<EdgeModelPrivate> d;
};
}

#endif
