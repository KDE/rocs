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

DocumentWrapper::DocumentWrapper(GraphDocumentPtr document, QJSEngine *engine)
    : QObject(engine)
    , m_document(document)
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

QJSEngine *DocumentWrapper::engine() const
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

QJSValue DocumentWrapper::node(int id) const
{
    // TODO average access time is linear, which might drastically be improved by
    //      using proper caching mechanisms at Document objects
    for (auto const &node : m_document->nodes()) {
        if (node->id() == id) {
            return m_engine->newQObject(nodeWrapper(node));
        }
    }
    QString command = QString("Document.node(%1)").arg(id);
    Q_EMIT message(i18nc("@info:shell", "%1: no node with ID %2 registered", command, id), GraphTheory::MessageType::ErrorMessage);
    return QJSValue();
}

QJSValue DocumentWrapper::nodes() const
{
    NodeList nodes = m_document->nodes();
    QJSValue array = m_engine->newArray(nodes.length());
    for (int i = 0; i < nodes.length(); ++i) {
        QJSValue nodeScriptValue = m_engine->newQObject(nodeWrapper(nodes.at(i)));
        array.setProperty(i, nodeScriptValue);
    }
    return array;
}

QJSValue DocumentWrapper::nodes(int type) const
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
        Q_EMIT message(i18nc("@info:shell", "%1: node type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return m_engine->newArray();
    }
    NodeList nodes = m_document->nodes(typePtr);
    QJSValue array = m_engine->newArray(nodes.length());
    for (int i = 0; i < nodes.length(); ++i) {
        QJSValue nodeScriptValue = m_engine->newQObject(nodeWrapper(nodes.at(i)));
        array.setProperty(i, nodeScriptValue);
    }
    return array;
}

QJSValue DocumentWrapper::edges() const
{
    EdgeList edges = m_document->edges();
    QJSValue array = m_engine->newArray(edges.length());
    for (int i = 0; i < edges.length(); ++i) {
        QJSValue edgeScriptValue = m_engine->newQObject(edgeWrapper(edges.at(i)));
        array.setProperty(i, edgeScriptValue);
    }
    return array;
}

QJSValue DocumentWrapper::edges(int type) const
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
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return m_engine->newArray();
    }
    EdgeList edges = m_document->edges(typePtr);
    QJSValue array = m_engine->newArray(edges.length());
    for (int i = 0; i < edges.length(); ++i) {
        QJSValue edgeScriptValue = m_engine->newQObject(edgeWrapper(edges.at(i)));
        array.setProperty(i, edgeScriptValue);
    }
    return array;
}

QJSValue DocumentWrapper::createNode(int x, int y)
{
    NodePtr node = Node::create(m_document);
    node->setX(x);
    node->setY(y);
    return m_engine->newQObject(nodeWrapper(node));
}

QJSValue DocumentWrapper::createEdge(NodeWrapper *from, NodeWrapper *to)
{
    if (!from) {
        QString command = QString("Document.createEdge(from, to)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"from\" is not a valid node object", command), GraphTheory::MessageType::ErrorMessage);
        return QJSValue();
    }
    if (!to) {
        QString command = QString("Document.createEdge(from, to)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"to\" is not a valid node object", command), GraphTheory::MessageType::ErrorMessage);
        return QJSValue();
    }
    EdgePtr edge = Edge::create(from->node(), to->node());
    return m_engine->newQObject(edgeWrapper(edge));
}

void DocumentWrapper::remove(const QJSValue &object)
{
    if (!object.isObject() || object.isNull()) {
        QString command = QString("Document.remove(obj)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"obj\" is not called on an object", command), GraphTheory::MessageType::ErrorMessage);
        return;
    }

    auto qobj = object.toQObject();
    if (auto edge = qobject_cast<EdgeWrapper *>(qobj)) {
        // note: The EdgeWrapper object explicitly is not removed from m_edgeMap and by this the edge object is not removed.
        // This has the benefit to not taking care validity of Edge, EdgeWrapper and its QScriptObject, but on the downside
        // leads to much used memory, that is only freed after run.
        // TODO: we need a mechanism that carefully implements on-the-fly object deletions
        edge->edge()->destroy();
    } else if (auto node = qobject_cast<NodeWrapper *>(qobj)) {
        // note: The NodeWrapper object explicitly is not removed from m_nodeMap and by this the node object is not removed.
        // This has the benefit to not taking care validity of Node, NodeWrapper and its QScriptObject, but on the downside
        // leads to much used memory, that is only freed after run.
        // TODO: we need a mechanism that carefully implements on-the-fly object deletions
        node->node()->destroy();
    } else {
        QString command = QString("Document.remove(obj)");
        Q_EMIT message(i18nc("@info:shell", "%1: \"obj\" is neither a valid node or edge object", command), GraphTheory::MessageType::ErrorMessage);
    }
}

#include "moc_documentwrapper.cpp"
