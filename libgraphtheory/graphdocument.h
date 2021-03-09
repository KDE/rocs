/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef GRAPHDOCUMENT_H
#define GRAPHDOCUMENT_H

#include "graphtheory_export.h"
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
    void nodeTypeAboutToBeAdded(NodeTypePtr,int);
    void nodeTypeAdded();
    void nodeTypesAboutToBeRemoved(int,int);
    void nodeTypesRemoved();
    void edgeTypeAboutToBeAdded(EdgeTypePtr,int);
    void edgeTypeAdded();
    void edgeTypesAboutToBeRemoved(int,int);
    void edgeTypesRemoved();

  /*
   * General document related properties
   */
public:
    /**
     * Get this document's name.
     * Returns a meaningful name like some unique "Untitled XYZ" for the document - \e without
     * documentUrl or basename for documents with url.
     * @return readable document name
     */
    QString documentName() const;

    void setDocumentName(const QString &name);

Q_SIGNALS:
    void documentNameChanged(const QString&);

  /*
   * General file related actions.
   * None of these actions cause user interaction.
   */
public:

    /**
     * Reload the current document.
     * @return @e true on success, i.e. the document was reloaded, otherwise
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

    /**
     * @return @e true if document has is modified, otherwise
     *         @e false
     */
    bool isModified() const;

    /**
     * Set modified state of the document to @p modified.
     */
    void setModified(bool modified);

Q_SIGNALS:
    void documentUrlChanged();
    void modifiedChanged();

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
