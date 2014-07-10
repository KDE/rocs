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

#ifndef GRAPHDOCUMENT_H
#define GRAPHDOCUMENT_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "node.h"
#include "edgetype.h"
#include "nodetype.h"

#include <QObject>
#include <QSharedPointer>
#include <QList>

namespace GraphTheory
{

class GraphDocumentPrivate;
class View;

/**
 * \class GraphDocument
 */
class GRAPHTHEORY_EXPORT GraphDocument : public QObject
{
    Q_OBJECT

public:
    /**
     * Creates a new GraphDocument.
     *
     * @return a GraphDocument object
     */
    static GraphDocumentPtr create();

    /** Destroys the document */
    virtual ~GraphDocument();

    /**
     * @return shared pointer to object
     */
    GraphDocumentPtr self() const;

    /**
     * Destroys the document object and all of its contents.
     */
    void destroy();

    View * createView(QWidget *parent);

    /**
     * @return list of nodes contained at the document
     */
    NodeList nodes(NodeTypePtr type = NodeTypePtr()) const;

    /**
     * @return list of edge contained at the document
     */
    EdgeList edges(EdgeTypePtr type = EdgeTypePtr()) const;

    /**
     * Add @p node to this document. The node must be correctly setup before, i.e.,
     * its type and document values have to be set. When inserting a node already in the list,
     * insert does nothing.
     *
     * @param node  the node to be added to the document
     */
    void insert(NodePtr node);

    /**
     * Add @p edge to this document. The edge must be correctly setup before, i.e.,
     * its type and end points have to be set. When inserting an edge already in the list,
     * insert does not perform any operation.
     *
     * @param edge  the edge to be added to the document
     */
    void insert(EdgePtr edge);

    /**
     * Add the NodeType @p type to this document.
     *
     * @param type  the NodeType to be added to the document
     */
    void insert(NodeTypePtr type);

    /**
     * Add the EdgeType @p type to this document.
     *
     * @param type  the EdgeType to be added to the document
     */
    void insert(EdgeTypePtr type);

    /**
     * Remove @p node from this document. If the node is valid, Node::destroy() will be called,
     * otherwise it will only be removed.
     *
     * @param node  the node to be removed from the document
     */
    void remove(NodePtr node);

    /**
     * Remove @p edge from this document. If the edge is valid, Edge::destroy() will be called,
     * otherwise it will only be removed.
     *
     * @param edge  the edge to be removed from the document
     */
    void remove(EdgePtr edge);

    /**
     * Remove @p type and all associated nodes from this document. If the type is valid,
     * NodeType::destroy() will be called.
     *
     * @param type  the node type to be removed from the document
     */
    void remove(NodeTypePtr type);

    /**
     * Remove @p type and all associated edge from this document. If the type is valid,
     * EdgeType::destroy() will be called.
     *
     * @param type  the edge type to be removed from the document
     */
    void remove(EdgeTypePtr type);

    /**
     * List of registered edge types. The list is never empty and the first element is the
     * default EdgeType.
     *
     * @return the list of edge types
     */
    QList<EdgeTypePtr> edgeTypes() const;

    /**
     * List of registered node types. The list is never empty and the first element is the
     * default NodeType.
     *
     * @return the list of edge types
     */
    QList<NodeTypePtr> nodeTypes() const;

    /**
     * Generate a unique unique identifier. This identifier is uniquely generated within this
     * document. But it is not checked, if it is already used for any node.
     * @return generated identifier
     */
    uint generateId();

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
    void nodeAboutToBeAdded(NodePtr,int);
    void nodeAdded();
    void nodesAboutToBeRemoved(int,int);
    void nodesRemoved();
    void edgeAboutToBeAdded(EdgePtr,int);
    void edgeAdded();
    void edgesAboutToBeRemoved(int,int);
    void edgesRemoved();

  /*
   * General file related actions.
   * None of these actions cause user interaction.
   */
public:
    /**
     * Reload the current document.
     * @return @e true on success, i.e. the the document was reloaded, otherwise
     *         @e false
     */
    bool documentReload();

    /**
     * Save document to path as given by GraphDocument::documentUrl()
     * @return @e true on success, i.e. the save has been done, otherwise
     *         @e false
     */
    bool documentSave();

    /**
     * Save document to path @p url, this does not change the documentUrl value.
     * To also change the document value, use setDocumentUrl(...).
     * @return @e true on success, i.e. the save has been done, otherwise
     *         @e false
     */
    bool documentSaveAs(const QUrl &documentUrl);

    /**
     * @return path used for saving
     */
    QUrl documentUrl() const;

    /**
     * Set file path used for saving.
     * \param documentUrl path to local file for saving the document
     */
    void setDocumentUrl(const QUrl &documentUrl);

    bool isModified();

Q_SIGNALS:
    void documentUrlChanged();

protected:
    GraphDocument();

private:
    Q_DISABLE_COPY(GraphDocument)
    const QScopedPointer<GraphDocumentPrivate> d;
    void setQpointer(GraphDocumentPtr q);
    static uint objectCounter;
};
}

#endif
