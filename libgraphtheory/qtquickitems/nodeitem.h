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

#ifndef NODEITEM_H
#define NODEITEM_H

#include "libgraphtheoryexport.h"
#include "node.h"
#include <QQuickPaintedItem>

namespace GraphTheory
{
class NodeItemPrivate;

class GRAPHTHEORY_EXPORT NodeItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(GraphTheory::Node * node READ node WRITE setNode NOTIFY nodeChanged)

public:
    explicit NodeItem(QQuickPaintedItem *parent = 0);
    virtual ~NodeItem();
    Node * node() const;
    void setNode(Node *node);
    /** reimplemented from QQuickPaintedItem **/
    void paint(QPainter *painter);

Q_SIGNALS:
    void nodeChanged();

private Q_SLOTS:
    void updateXfromScene();
    void updateYfromScene();
    void setPosition(const QPointF& position);

private:
    Q_DISABLE_COPY(NodeItem)
    const QScopedPointer<NodeItemPrivate> d;
};
}

#endif
