/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EDGETYPEMODEL_H
#define EDGETYPEMODEL_H

#include "libgraphtheoryexport.h"
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
        DataRole                        //!< access to EdgeType object
    };

    explicit EdgeTypeModel(QObject *parent = 0);
    ~EdgeTypeModel();
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    virtual QHash<int,QByteArray> roleNames() const;
    void setDocument(GraphDocumentPtr document);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

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
