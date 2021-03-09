/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef EDGETYPEMODEL_H
#define EDGETYPEMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"

#include <QAbstractListModel>

class QSignalMapper;

namespace GraphTheory
{
class GraphDocument;
class EdgeTypeModelPrivate;

class GRAPHTHEORY_EXPORT EdgeTypeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum EdgeRoles {
        IdRole = Qt::UserRole + 1,      //!< unique identifier of edge
        TitleRole,                      //!< title of edge type
        ColorRole,                      //!< color of edge type
        DirectionRole,                  //!< direction of edge type
        DataRole                        //!< access to EdgeType object
    };

    explicit EdgeTypeModel(QObject *parent = 0);
    virtual ~EdgeTypeModel();
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    virtual QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    void setDocument(GraphDocumentPtr document);
    EdgeTypePtr type(int index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void edgeChanged(int index);

private Q_SLOTS:
    void onEdgeTypeAboutToBeAdded(EdgeTypePtr edge, int index);
    void onEdgeTypeAdded();
    void onEdgeTypesAboutToBeRemoved(int first, int last);
    void onEdgeTypesRemoved();
    void emitEdgeTypeChanged(int row);

private:
    Q_DISABLE_COPY(EdgeTypeModel)
    const QScopedPointer<EdgeTypeModelPrivate> d;
};
}

#endif
