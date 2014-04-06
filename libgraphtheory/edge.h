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

#ifndef EDGE_H
#define EDGE_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "edgetype.h"
#include "node.h"

#include <QObject>
#include <QSharedPointer>

namespace GraphTheory
{

class EdgePrivate;

/**
 * \class Edge
 */
class GRAPHTHEORY_EXPORT Edge : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new Edge from @p from to @p to. The order of @p from and @p to will be preserved
     * even when the edge is bidirectional, to allow unique access to the corresponding nodes.
     *
     * @param from  the Node this edge points from
     * @param to    the Node this edge points to
     * @return a Edge object
     */
    static EdgePtr create(NodePtr from, NodePtr to);

    /** Destroys the edge */
    virtual ~Edge();

    /**
     * Destroys the edge object and removes it from the connected nodes.
     */
    void destroy();

    /**
     * The valid state returns @c true if the object is created and can be used, @c false if it is
     * not yet created or about to be removed
     *
     * @return the validity state of the object
     */
    bool isValid() const;

    /**
     * @return the Node the edge points from
     */
    NodePtr from() const;

    /**
     * @return the Node the edge points to
     */
    NodePtr to() const;

    /**
     * Return the EdgeType of the edge. This value is always valid.
     *
     * @return the EdgeType of the edge
     */
    EdgeTypePtr type() const;

    /**
     * Set EdgeType for the edge. Setting this emits signal
     * typeChanged(@p type). The @p type must be registered at the GraphDocument that contains the
     * edge.
     *
     * @param type the type of the edge
     */
    void setType(EdgeTypePtr type);

    /**
     * Debug method that tracks how many node objects exist.
     *
     * @return number of edge objects
     */
    static uint objects()
    {
        return objectCounter;
    }

Q_SIGNALS:
    void typeChanged(EdgeTypePtr type);

protected:
    Edge();

private:
    Q_DISABLE_COPY(Edge)
    const QScopedPointer<EdgePrivate> d;
    void setQpointer(EdgePtr q);
    static uint objectCounter;
};
}

#endif
