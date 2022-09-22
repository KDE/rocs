/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "documentwrapper.h"
#include "edgewrapper.h"
#include "nodetype.h"
#include "nodewrapper.h"
#include <KLocalizedString>
#include <QDebug>

using namespace GraphTheory;

DocumentWrapper::DocumentWrapper(GraphDocumentPtr document, QScriptEngine *engine)
    : m_document(document)
    , m_engine(engine)
{
    const auto nodes = document->nodes();
    for (const NodePtr &node : nodes) {
        registerWrapper(node);
    }
    const auto edges = document->edges();
    for (const EdgePtr &edge : edges) {
        registerWrapper(edge);
    }

    connect(document.data(), &GraphDocument::nodeAboutToBeAdded, this, static_cast<void (DocumentWrapper::*)(NodePtr)>(&DocumentWrapper::registerWrapper));
    connect(document.data(), &GraphDocument::edgeAboutToBeAdded, this, static_cast<void (DocumentWrapper::*)(EdgePtr)>(&DocumentWrapper::registerWrapper));
}

DocumentWrapper::~DocumentWrapper()
{
    qDeleteAll(m_edgeMap);
    qDeleteAll(m_nodeMap);
}

QScriptEngine *DocumentWrapper::engine() const
{
    return m_engine;
}

void DocumentWrapper::registerWrapper(NodePtr node)
{
    if (m_nodeMap.contains(node)) {
        return;
    }
    NodeWrapper *wrapper = new NodeWrapper(node, this);
    m_nodeMap.insert(node, wrapper);
    connect(wrapper, &NodeWrapper::message, this, &DocumentWrapper::message);
    return;
}

void DocumentWrapper::registerWrapper(EdgePtr edge)
{
    if (m_edgeMap.contains(edge)) {
        return;
    }
    EdgeWrapper *wrapper = new EdgeWrapper(edge, this);
    m_edgeMap.insert(edge, wrapper);
    connect(wrapper, &EdgeWrapper::message, this, &DocumentWrapper::message);
    return;
}

NodeWrapper *DocumentWrapper::nodeWrapper(NodePtr node) const
{
    Q_ASSERT(m_nodeMap.contains(node));
    return m_nodeMap.value(node);
}

EdgeWrapper *DocumentWrapper::edgeWrapper(EdgePtr edge) const
{
    Q_ASSERT(m_edgeMap.contains(edge));
    return m_edgeMap.value(edge);
}

QScriptValue DocumentWrapper::node(int id) const
{
    // TODO average access time is linear, which might drastically be improved by
    //      using proper caching mechanisms at Document objects
    for (auto const &node : m_document->nodes()) {
        if (node->id() == id) {
            return m_engine->newQObject(nodeWrapper(node), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
        }
    }
    QString command = QString("Document.node(%1)").arg(id);
    Q_EMIT message(i18nc("@info:shell", "%1: no node with ID %2 registered", command, id), Kernel::ErrorMessage);
    return QScriptValue();
}

QScriptValue DocumentWrapper::nodes() const
{
    NodeList nodes = m_document->nodes();
    QScriptValue array = m_engine->newArray(nodes.length());
    for (int i = 0; i < nodes.length(); ++i) {
        QScriptValue nodeScriptValue = m_engine->newQObject(nodeWrapper(nodes.at(i)), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
        array.setProperty(i, nodeScriptValue);
    }
    return array;
}

QScriptValue DocumentWrapper::nodes(int type) const
{
    NodeTypePtr typePtr;
    const auto nodeTypes = m_document->nodeTypes();
    for (const NodeTypePtr &typeTest : nodeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("Document.nodes(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: node type ID %2 not registered", command, type), Kernel::ErrorMessage);
        return m_engine->newArray();
    }
    NodeList nodes = m_document->nodes(typePtr);
    QScriptValue array = m_engine->newArray(nodes.length());
    for (int i = 0; i < nodes.length(); ++i) {
        QScriptValue nodeScriptValue = m_engine->newQObject(nodeWrapper(nodes.at(i)), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
        array.setProperty(i, nodeScriptValue);
    }
    return array;
}

QScriptValue DocumentWrapper::edges() const
{
    EdgeList edges = m_document->edges();
    QScriptValue array = m_engine->newArray(edges.length());
    for (int i = 0; i < edges.length(); ++i) {
        QScriptValue edgeScriptValue = m_engine->newQObject(edgeWrapper(edges.at(i)), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
        array.setProperty(i, edgeScriptValue);
    }
    return array;
}

QScriptValue DocumentWrapper::edges(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_document->edgeTypes();
    for (const EdgeTypePtr &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("Document.edges(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), Kernel::ErrorMessage);
        return m_engine->newArray();
    }
    EdgeList edges = m_document->edges(typePtr);
    QScriptValue array = m_engine->newArray(edges.length());
    for (int i = 0; i < edges.length(); ++i) {
        QScriptValue edgeScriptValue = m_engine->newQObject(edgeWrapper(edges.at(i)), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
        array.setProperty(i, edgeScriptValue);
    }
    return array;
}

QScriptValue DocumentWrapper::createNode(int x, int y)
{
    NodePtr node = Node::create(m_document);
    node->setX(x);
    node->setY(y);
    return m_engine->newQObject(nodeWrapper(node), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
}

QScriptValue DocumentWrapper::createEdge(NodeWrapper *from, NodeWrapper *to)
{
    if (!from) {
        QString command = QString("Document.createEdge(from, to)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"from\" is not a valid node object", command), Kernel::ErrorMessage);
        return QScriptValue();
    }
    if (!to) {
        QString command = QString("Document.createEdge(from, to)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"to\" is not a valid node object", command), Kernel::ErrorMessage);
        return QScriptValue();
    }
    EdgePtr edge = Edge::create(from->node(), to->node());
    return m_engine->newQObject(edgeWrapper(edge), QScriptEngine::QtOwnership, QScriptEngine::AutoCreateDynamicProperties);
}

void DocumentWrapper::remove(NodeWrapper *node)
{
    if (!node) {
        QString command = QString("Document.remove(node)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"node\" is not a valid node object", command), Kernel::ErrorMessage);
        return;
    }
    // note: The NodeWrapper object explicitly is not removed from m_nodeMap and by this the node object is not removed.
    // This has the benefit to not taking care validity of Node, NodeWrapper and its QScriptObject, but on the downside
    // leads to much used memory, that is only freed after run.
    // TODO: we need a mechanism that carefully implements on-the-fly object deletions
    node->node()->destroy();
}

void DocumentWrapper::remove(EdgeWrapper *edge)
{
    if (!edge) {
        QString command = QString("Document.remove(edge)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"edge\" is not a valid edge object", command), Kernel::ErrorMessage);
        return;
    }
    // note: The EdgeWrapper object explicitly is not removed from m_edgeMap and by this the edge object is not removed.
    // This has the benefit to not taking care validity of Edge, EdgeWrapper and its QScriptObject, but on the downside
    // leads to much used memory, that is only freed after run.
    // TODO: we need a mechanism that carefully implements on-the-fly object deletions
    edge->edge()->destroy();
}
