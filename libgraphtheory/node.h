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

#ifndef NODE_H
#define NODE_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "graphdocument.h"

#include <QObject>

namespace GraphTheory
{
class NodePrivate;

/**
 * \class Node
 */
class GRAPHTHEORY_EXPORT Node : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new Node.
     *
     * @param document  the GraphDocument containing this node
     * @return a Node object
     */
    static NodePtr create(GraphDocumentPtr document);

    /** Destroys the node */
    virtual ~Node();

    /**
     * Destroys the node object, invoke destroy calls for edges, and removes it from the document.
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
     * @return the GraphDocument that contains this node
     */
    GraphDocumentPtr document() const;

    /**
     * Return the NodeType of the node. This value is always valid.
     *
     * @return the NodeType of the node
     */
    NodeTypePtr type() const;

    /**
     * Set NodeType for the node. Setting this emits signal
     * typeChanged(@p type). The @p type must be registered at the GraphDocument that contains the
     * node.
     *
     * @param type the type of the node
     */
    void setType(NodeTypePtr type);

    /**
     * Add @p edge to this node. The edge must connect this node to another node (can be smame).
     * When inserting the edge, the end points and the type of the edge must be set.
     *
     * @param edge  the edge to be added to the node
     */
    void insert(EdgePtr edge);

    /**
     * Remove @p edge from this node. If the edge is valid, Edge::destroy() will be called,
     * otherwise it will only be removed.
     *
     * @param edge  the edge to be removed from the node
     */
    void remove(EdgePtr edge);

    /**
     * @return edges adjacent to this node
     */
    EdgeList edges() const;

    /**
     * Debug method that tracks how many node objects exist.
     *
     * @return number of node objects
     */
    static uint objects()
    {
        return objectCounter;
    }

Q_SIGNALS:
    void typeChanged(NodeTypePtr type);
    void edgeAdded(EdgePtr edge);

protected:
    Node();

private:
    Q_DISABLE_COPY(Node)
    const QScopedPointer<NodePrivate> d;
    void setQpointer(NodePtr q);
    static uint objectCounter;
};
}

#endif
