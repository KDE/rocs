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

#ifndef NODEMODEL_H
#define NODEMODEL_H

#include "graphtheory_export.h"
#include "typenames.h"

#include <QAbstractListModel>


class QSignalMapper;

namespace GraphTheory
{
class GraphDocument;
class NodeModelPrivate;

class GRAPHTHEORY_EXPORT NodeModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum NodeRoles {
        IdRole = Qt::UserRole + 1,      //!< unique identifier of node
        DataRole                        //!< access to Node object
    };

    explicit NodeModel(QObject *parent = 0);
    virtual ~NodeModel();
    /**
     * Reimplemented from QAbstractListModel::roleNames()
     */
    virtual QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    void setDocument(GraphDocumentPtr document);
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void nodeChanged(int index);

private Q_SLOTS:
    void onNodeAboutToBeAdded(NodePtr node, int index);
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
