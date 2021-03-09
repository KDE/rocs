/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "graphdocument.h"
#include "view.h"
#include "edge.h"
#include "fileformats/fileformatmanager.h"
#include "logging_p.h"
#include <KLocalizedString>
#include <QSurfaceFormat>
#include <QString>

using namespace GraphTheory;

// initialize number of edge objects
uint GraphDocument::objectCounter = 0;

class GraphTheory::GraphDocumentPrivate {
public:
    GraphDocumentPrivate()
        : m_valid(false)
        , m_view(0)
        , m_documentUrl(QUrl())
        , m_name(QString())
        , m_lastGeneratedId(0)
        , m_modified(false)
    {
    }

    ~GraphDocumentPrivate()
    {
    }

    GraphDocumentPtr q;
    bool m_valid;
    View *m_view;
    QList<EdgeTypePtr> m_edgeTypes;
    QList<NodeTypePtr> m_nodeTypes;
    NodeList m_nodes;
    EdgeList m_edges;

    QUrl m_documentUrl;
    QString m_name;
    uint m_lastGeneratedId;
    bool m_modified;
};

GraphDocumentPtr GraphDocument::self() const
{
    return d->q;
}

void GraphDocument::destroy()
{
    d->m_valid = false;
    foreach (EdgePtr edge, d->m_edges) {
        edge->destroy();
    }
    d->m_edges.clear();
    foreach (NodePtr node, d->m_nodes) {
        node->destroy();
    }
    d->m_nodes.clear();
    foreach (NodeTypePtr type, d->m_nodeTypes) {
        type->destroy();
    }
    d->m_nodeTypes.clear();
    foreach (EdgeTypePtr type, d->m_edgeTypes) {
        type->destroy();
    }
    d->m_edgeTypes.clear();

    // reset last reference to this object
    d->q.reset();
}

GraphDocument::GraphDocument()
    : QObject()
    , d(new GraphDocumentPrivate)
{
    ++GraphDocument::objectCounter;
}

GraphDocument::~GraphDocument()
{

    --GraphDocument::objectCounter;
}

View * GraphDocument::createView(QWidget *parent)
{
    if (d->m_view) {
        return d->m_view;
    }
    d->m_view = new View(parent);
    d->m_view->setGraphDocument(d->q);

    // apply antialiasing on the view
    QSurfaceFormat format = d->m_view->format();
    format.setSamples(16);
    d->m_view->setFormat(format);

    return d->m_view;
}

GraphDocumentPtr GraphDocument::create()
{
    GraphDocumentPtr pi(new GraphDocument);
    pi->setQpointer(pi);

    // create default edge unidirectional
    EdgeTypePtr edgeTypeUnidirectional = EdgeType::create(pi);
    edgeTypeUnidirectional->setName(i18n("unidirectional"));
    edgeTypeUnidirectional->setDirection(EdgeType::Direction::Unidirectional);

    // create default edge bidirectional
    EdgeTypePtr edgeTypeBidirectional = EdgeType::create(pi);
    edgeTypeBidirectional->setName(i18n("bidirectional"));
    edgeTypeBidirectional->setDirection(EdgeType::Direction::Bidirectional);

    // create default node type
    NodeType::create(pi)->setName(i18n("default"));
    pi->d->m_valid = true;
    pi->d->m_modified = false;
    return pi;
}

NodeList GraphDocument::nodes(NodeTypePtr type) const
{
    if (!type) {
        return d->m_nodes;
    }

    NodeList nodes;
    foreach (NodePtr node, d->m_nodes) {
        if (node->type() == type) {
            nodes.append(node);
        }
    }
    return nodes;
}

EdgeList GraphDocument::edges(EdgeTypePtr type) const
{
    if (!type) {
        return d->m_edges;
    }

    EdgeList edges;
    foreach (EdgePtr edge, d->m_edges) {
        if (edge->type() == type) {
            edges.append(edge);
        }
    }
    return edges;
}

void GraphDocument::insert(NodePtr node)
{
    Q_ASSERT(node);
    Q_ASSERT(node->document() == d->q);

    if (!node || d->m_nodes.contains(node)) {
        return;
    }
    if (0 <= node->id() && (uint)node->id() < d->m_lastGeneratedId) {
        d->m_lastGeneratedId = node->id();
    }

    emit nodeAboutToBeAdded(node, d->m_nodes.length());
    d->m_nodes.append(node);
    emit nodeAdded();
    setModified(true);
}

void GraphDocument::insert(EdgePtr edge)
{
    Q_ASSERT(edge);
    Q_ASSERT(edge->from()->document() == d->q);
    Q_ASSERT(edge->to()->document() == d->q);

    if (!edge || d->m_edges.contains(edge)) {
        return;
    }

    emit edgeAboutToBeAdded(edge, d->m_edges.length());
    d->m_edges.append(edge);
    emit edgeAdded();
    setModified(true);
}

void GraphDocument::insert(NodeTypePtr type)
{
    Q_ASSERT(type);
    if (d->m_nodeTypes.contains(type)) {
        return;
    }
    if (0 <= type->id() && (uint)type->id() < d->m_lastGeneratedId) {
        d->m_lastGeneratedId = type->id();
    }
    emit nodeTypeAboutToBeAdded(type, d->m_nodeTypes.length());
    d->m_nodeTypes.append(type);
    emit nodeTypeAdded();
    setModified(true);
}

void GraphDocument::insert(EdgeTypePtr type)
{
    Q_ASSERT(type);
    if (d->m_edgeTypes.contains(type)) {
        return;
    }
    if (0 <= type->id() && (uint)type->id() < d->m_lastGeneratedId) {
        d->m_lastGeneratedId = type->id();
    }
    emit edgeTypeAboutToBeAdded(type, d->m_edgeTypes.length());
    d->m_edgeTypes.append(type);
    emit edgeTypeAdded();
    setModified(true);
}

void GraphDocument::remove(NodePtr node)
{
    if (node->isValid()) {
        node->destroy();
    }
    int index = d->m_nodes.indexOf(node);
    if (index >= 0) {
        emit nodesAboutToBeRemoved(index,index);
        d->m_nodes.removeAt(index);
        emit nodesRemoved();
    }
    setModified(true);
}

void GraphDocument::remove(EdgePtr edge)
{
    if (edge->isValid()) {
        edge->destroy();
    }
    int index = d->m_edges.indexOf(edge);
    if (index >= 0) {
        emit edgesAboutToBeRemoved(index,index);
        d->m_edges.removeAt(index);
        emit edgesRemoved();
    }
    setModified(true);
}

void GraphDocument::remove(NodeTypePtr type)
{
    foreach (NodePtr node, d->m_nodes) {
        if (node->type() == type) {
            node->destroy();
        }
    }
    if (type->isValid()) {
        type->destroy();
    }
    int index = d->m_nodeTypes.indexOf(type);
    emit nodeTypesAboutToBeRemoved(index, index);
    d->m_nodeTypes.removeOne(type);
    emit nodeTypesRemoved();
    setModified(true);
}

void GraphDocument::remove(EdgeTypePtr type)
{
    foreach (EdgePtr edge, edges(type)) {
        edge->destroy();
    }
    if (type->isValid()) {
        type->destroy();
    }
    int index = d->m_edgeTypes.indexOf(type);
    emit edgeTypesAboutToBeRemoved(index, index);
    d->m_edgeTypes.removeOne(type);
    emit edgeTypesRemoved();
    setModified(true);
}

QList< EdgeTypePtr > GraphDocument::edgeTypes() const
{
    return d->m_edgeTypes;
}

QList< NodeTypePtr > GraphDocument::nodeTypes() const
{
    return d->m_nodeTypes;
}

uint GraphDocument::generateId()
{
    return ++d->m_lastGeneratedId;
}

void GraphDocument::setQpointer(GraphDocumentPtr q)
{
    d->q = q;
}

//BEGIN file stuff
QString GraphDocument::documentName() const
{
    if (d->m_name.isEmpty()) {
        return i18nc("@title:tab initial title for graph document", "New Graph");
    }
    return d->m_name;
}

void GraphDocument::setDocumentName(const QString& name)
{
    if (name == d->m_name) {
        return;
    }
    d->m_name = name;
    emit documentNameChanged(name);
    setModified(true);
}
//END file stuff


//BEGIN file stuff
bool GraphDocument::documentReload()
{
    qCCritical(GRAPHTHEORY_GENERAL) << "graph reloading not implemented!";
    //FIXME reload document
    setModified(false);
    return true;
}

bool GraphDocument::documentSave()
{
    return documentSaveAs(d->m_documentUrl);
}

bool GraphDocument::documentSaveAs(const QUrl &documentUrl)
{
    if (!documentUrl.isValid()) {
        qCCritical(GRAPHTHEORY_GENERAL) << "No valid document url specified, abort saving.";
        return false;
    }

    FileFormatManager fileFormatManager;
    FileFormatInterface *serializer = fileFormatManager.defaultBackend();
    serializer->setFile(documentUrl);
    serializer->writeFile(d->q);
    if (serializer->hasError()) {
        qCCritical(GRAPHTHEORY_GENERAL) << "Graph file serializer reported error:" << serializer->errorString();
        return false;
    }

    // update document path if necessary
    if (d->m_documentUrl != documentUrl) {
        d->m_documentUrl = documentUrl;
        emit documentUrlChanged();
    }
    setModified(false);

    return true;
}

QUrl GraphDocument::documentUrl() const
{
    return d->m_documentUrl;
}

void GraphDocument::setDocumentUrl(const QUrl &documentUrl)
{
    d->m_documentUrl = documentUrl;
    emit documentUrlChanged();
}

bool GraphDocument::isModified() const
{
    return d->m_modified;
}

void GraphDocument::setModified(bool modified)
{
    if (modified == d->m_modified) {
        return;
    }
    d->m_modified = modified;
    emit modifiedChanged();
}

//END
