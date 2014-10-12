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

#ifndef NODEWRAPPER_H
#define NODEWRAPPER_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "node.h"
#include "graphdocument.h"
#include "documentwrapper.h"

#include <QtScript>
#include <QObject>
#include <QColor>

namespace GraphTheory
{
class NodeWrapperPrivate;

/**
 * \class NodeWrapper
 * Wraps NodePtr to be accessible via QtScript. All properties of the node object are available
 * as commong QObject properties.
 */
class NodeWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY positionChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY positionChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
    NodeWrapper(NodePtr node, DocumentWrapper *documentWrapper);

    virtual ~NodeWrapper();

    /**
     * If the id value is invalid, -1 is returned.
     *
     * @return node identifier
     */
    int id() const;

    /**
     * @return x-position of node
     */
    qreal x() const;

    /**
     * set x-position of node to @c x
     */
    void setX(qreal x);

    /**
     * @return y-position of node
     */
    qreal y() const;

    /**
     * set y-position of node to @c y
     */
    void setY(qreal y);

    /**
     * @return color of node
     */
    QColor color() const;

    /**
     * set color of node to @c color
     */
    void setColor(const QColor &color);

    /**
     * @return NodeType::id of corresponding node
     */
    int type() const;

    /**
     * Set NodeType of corresponding node by specifying its ID by @p typeId.
     * If @p typeId does not name the ID of any NodeType, the type is not changed.
     */
    void setType(int typeId);

    Q_INVOKABLE QList<GraphTheory::EdgeWrapper*> edges() const;
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper*> inEdges() const;
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper*> outEdges() const;

    /** reimplemented from QObject **/
    virtual bool event(QEvent* e);

public Q_SLOTS:
    void updateDynamicProperties();

Q_SIGNALS:
    void idChanged(int id);
    void positionChanged(const QPointF &position);
    void colorChanged(const QColor &color);
    void typeChanged();

private:
    Q_DISABLE_COPY(NodeWrapper)
    const NodePtr m_node;
    const DocumentWrapper *m_documentWrapper;
};
}

Q_DECLARE_METATYPE(QList<GraphTheory::NodeWrapper*>)
Q_DECLARE_METATYPE(GraphTheory::NodeWrapper*)

#endif
