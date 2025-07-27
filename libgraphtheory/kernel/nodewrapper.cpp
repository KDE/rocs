/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodewrapper.h"
#include "edge.h"
#include "edgewrapper.h"
#include "nodetype.h"
#include <KLocalizedString>
#include <QDebug>
#include <QEvent>
#include <QPointF>

using namespace GraphTheory;

NodeWrapper::NodeWrapper(NodePtr node, DocumentWrapper *documentWrapper)
    : QObject(node.data())
    , m_node(node)
    , m_documentWrapper(documentWrapper)
{
    connect(m_node.data(), &Node::idChanged, this, &NodeWrapper::idChanged);
    connect(m_node.data(), &Node::colorChanged, this, &NodeWrapper::colorChanged);
    connect(m_node.data(), &Node::positionChanged, this, &NodeWrapper::positionChanged);
    connect(m_node.data(), &Node::dynamicPropertiesChanged, this, &NodeWrapper::updateDynamicProperties);
    connect(m_node.data(), &Node::typeChanged, this, &NodeWrapper::typeChanged);

    updateDynamicProperties();
}

NodeWrapper::~NodeWrapper()
{
}

NodePtr NodeWrapper::node() const
{
    return m_node;
}

int NodeWrapper::id() const
{
    return m_node->id();
}

qreal NodeWrapper::x() const
{
    return m_node->x();
}

void NodeWrapper::setX(qreal x)
{
    if (x == NodeWrapper::x()) {
        return;
    }
    m_node->setX(x);
    // change signal will be emitted by connection to m_node signal
}

qreal NodeWrapper::y() const
{
    return m_node->y();
}

void NodeWrapper::setY(qreal y)
{
    if (y == NodeWrapper::y()) {
        return;
    }
    m_node->setY(y);
    // change signal will be emitted by connection to m_node signal
}

QString NodeWrapper::color() const
{
    return m_node->color().name();
}

void NodeWrapper::setColor(const QString &colorString)
{
    QColor color = QColor(colorString);
    if (color == m_node->color()) {
        return;
    }
    m_node->setColor(color);
    // change signal will be emitted by connection to m_node signal
}

int NodeWrapper::type() const
{
    return m_node->type()->id();
}

void NodeWrapper::setType(int typeId)
{
    NodeTypePtr newType = m_node->type();
    if (newType->id() == typeId) {
        return;
    }
    const auto nodeTypes = m_node->document()->nodeTypes();
    for (const auto &type : nodeTypes) {
        if (type->id() == typeId) {
            newType = type;
            break;
        }
    }
    if (newType == m_node->type()) {
        QString command = QString("node.type = %1)").arg(typeId);
        Q_EMIT message(i18nc("@info:shell", "%1: node type ID %2 not registered", command, typeId), GraphTheory::MessageType::ErrorMessage);
        return;
    }
    m_node->setType(newType);
    // change signal will be emitted by connection to m_node signal
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::edges() const
{
    QList<EdgeWrapper *> edges;
    const auto nodeEdges = m_node->edges();
    for (const auto &edge : nodeEdges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::edges(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("node.edges(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<EdgeWrapper *>();
    }
    QList<EdgeWrapper *> edges;
    const auto nodeEges = m_node->edges(typePtr);
    for (const auto &edge : nodeEges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::inEdges() const
{
    QList<EdgeWrapper *> edges;
    const auto inEdges = m_node->inEdges();
    for (const auto &edge : inEdges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::inEdges(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        ;
        QString command = QString("node.inEdges(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<EdgeWrapper *>();
    }
    QList<EdgeWrapper *> edges;
    const auto inEdges = m_node->inEdges(typePtr);
    for (const auto &edge : inEdges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::outEdges() const
{
    QList<EdgeWrapper *> edges;
    const auto outEdges = m_node->outEdges();
    for (const auto &edge : outEdges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<GraphTheory::EdgeWrapper *> NodeWrapper::outEdges(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("node.outEdges(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<EdgeWrapper *>();
    }
    QList<EdgeWrapper *> edges;
    const auto outEdges = m_node->outEdges(typePtr);
    for (const auto &edge : outEdges) {
        edges.append(m_documentWrapper->edgeWrapper(edge));
    }
    return edges;
}

QList<NodeWrapper *> NodeWrapper::neighbors() const
{
    QSet<NodeWrapper *> neighbors;
    const auto edges = m_node->edges();
    for (const auto &edge : edges) {
        if (m_node == edge->from()) {
            neighbors.insert(m_documentWrapper->nodeWrapper(edge->to()));
        } else {
            neighbors.insert(m_documentWrapper->nodeWrapper(edge->from()));
        }
    }
    return neighbors.values();
}

QList<NodeWrapper *> NodeWrapper::neighbors(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("node.neighbors(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<NodeWrapper *>();
    }
    QSet<NodeWrapper *> neighbors;
    const auto edges = m_node->edges(typePtr);
    for (const auto &edge : edges) {
        if (m_node == edge->from()) {
            neighbors.insert(m_documentWrapper->nodeWrapper(edge->to()));
        } else {
            neighbors.insert(m_documentWrapper->nodeWrapper(edge->from()));
        }
    }
    return neighbors.values();
}

QList<NodeWrapper *> NodeWrapper::predecessors() const
{
    QSet<NodeWrapper *> precessors;
    const auto inEdges = m_node->inEdges();
    for (const auto &edge : inEdges) {
        if (edge->type()->direction() == EdgeType::Unidirectional) {
            precessors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            continue;
        } else {
            if (m_node == edge->from()) {
                precessors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            } else {
                precessors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            }
        }
    }
    return precessors.values();
}

QList<NodeWrapper *> NodeWrapper::predecessors(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("node.predecessors(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<NodeWrapper *>();
    }
    QSet<NodeWrapper *> precessors;
    const auto inEdges = m_node->inEdges(typePtr);
    for (const auto &edge : inEdges) {
        if (edge->type()->direction() == EdgeType::Unidirectional) {
            precessors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            continue;
        } else {
            if (m_node == edge->from()) {
                precessors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            } else {
                precessors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            }
        }
    }
    return precessors.values();
}

QList<NodeWrapper *> NodeWrapper::successors() const
{
    QSet<NodeWrapper *> successors;
    const auto outEdges = m_node->outEdges();
    for (const auto &edge : outEdges) {
        if (edge->type()->direction() == EdgeType::Unidirectional) {
            successors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            continue;
        } else {
            if (m_node == edge->from()) {
                successors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            } else {
                successors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            }
        }
    }
    return successors.values();
}

QList<NodeWrapper *> NodeWrapper::successors(int type) const
{
    EdgeTypePtr typePtr;
    const auto edgeTypes = m_node->document()->edgeTypes();
    for (const auto &typeTest : edgeTypes) {
        if (typeTest->id() == type) {
            typePtr = typeTest;
            break;
        }
    }
    if (!typePtr) {
        QString command = QString("node.successors(%1)").arg(type);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, type), GraphTheory::MessageType::ErrorMessage);
        return QList<NodeWrapper *>();
    }
    QSet<NodeWrapper *> successors;
    const auto outEdges = m_node->outEdges(typePtr);
    for (const auto &edge : outEdges) {
        if (edge->type()->direction() == EdgeType::Unidirectional) {
            successors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            continue;
        } else {
            if (m_node == edge->from()) {
                successors.insert(m_documentWrapper->nodeWrapper(edge->to()));
            } else {
                successors.insert(m_documentWrapper->nodeWrapper(edge->from()));
            }
        }
    }
    return successors.values();
}

QJSValue NodeWrapper::toScriptValue(QJSEngine *engine)
{
    if (m_scriptValue) {
        return m_scriptValue.value();
    }
    if (!engine) {
        qCritical() << "cannot create QJSValue, engine is null";
        return QJSValue();
    }

    m_scriptValue = engine->newQObject(this);
    const auto dynamicPropertyNames = m_node->dynamicProperties();
    for (const auto &propertyName : dynamicPropertyNames) {
        m_scriptValue->setProperty(propertyName, engine->toScriptValue(m_node->dynamicProperty(propertyName)));
    }
    return m_scriptValue.value();
}

void NodeWrapper::releaseScriptValue()
{
    if (!m_scriptValue.has_value()) {
        return;
    }
    const auto dynamicPropertyNames = m_node->dynamicProperties();
    for (const auto &propertyName : dynamicPropertyNames) {
        QJSValue value = m_scriptValue->property(propertyName);
        m_node->setDynamicProperty(propertyName, value.toVariant());
    }
}

QJSValue NodeWrapper::distance(const QString &lengthProperty, QList<NodeWrapper *> targets)
{
    // TODO at a later point in time:
    //      factor this algorithm out into a self-contained graph algorithm class

    // Implementation of Floyd-Warshall Algorithm:
    // asymptotic runtime: O(n^3), n = number of nodes
    //
    // 1 let D be a |V| × |V| matrix of minimum distances initialized to ∞ (infinity)
    // 2 foreach(vertex v)
    // 3    D[v][v] ← 0
    // 4 foreach (edge (u,v))
    // 5    D[u][v] := w(u,v)  // the weight of the edge (u,v)
    // 6 for (k from 1 to |V|)
    // 7    for (i from 1 to |V|)
    // 8       for (j from 1 to |V|)
    // 9          if (D[i][j] > D[i][k] + D[k][j])
    // 10             D[i][j] := D[i][k] + D[k][j]
    // 11         end if

    const NodeList nodes = m_node->document()->nodes();
    const EdgeList edges = m_node->document()->edges();
    const int n = nodes.length();

    // create fast access mapping of node IDs to positions
    QMap<int, int> map;
    for (int i = 0; i < n; ++i) {
        map.insert(nodes.at(i)->id(), i);
    }

    // initialize distance matrix D
    QVector<QVector<qreal>> D(n, QVector<qreal>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                D[i][j] = 0;
            } else {
                D[i][j] = std::numeric_limits<qreal>::max();
            }
        }
    }

    // set edges lengths to D
    for (const auto &edge : edges) {
        D[map[edge->from()->id()]][map[edge->to()->id()]] = edge->dynamicProperty(lengthProperty).toDouble();
        if (edge->type()->direction() == EdgeType::Bidirectional) {
            D[map[edge->to()->id()]][map[edge->from()->id()]] = edge->dynamicProperty(lengthProperty).toDouble();
        }
    }

    // computation
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (D[i][j] > D[i][k] + D[k][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                }
            }
        }
    }

    // compute return statement
    QJSValue array = m_documentWrapper->engine()->newArray(targets.length());
    const int from = map[m_node->id()];
    for (int i = 0; i < targets.length(); ++i) {
        const qreal distance = D[from][map[targets.at(i)->id()]];
        array.setProperty(i, distance);
    }
    return array;
}

bool NodeWrapper::event(QEvent *e)
{
    if (e->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent *>(e);
        QString name = QString::fromUtf8(propertyEvent->propertyName());
        QVariant value = property(propertyEvent->propertyName());
        // only propagate property to node object if it is registered
        if (m_node->dynamicProperties().contains(name)) {
            m_node->setDynamicProperty(name, value);
        }
        return true;
    }
    return QObject::event(e);
}

void NodeWrapper::updateDynamicProperties()
{
    const auto dynamicProperties = m_node->dynamicProperties();
    for (const auto &property : dynamicProperties) {
        // property value must not be set to QVariant::Invalid, else the properties are not accessible
        // from the script engine
        if (m_node->dynamicProperty(property).isValid()) {
            setProperty(property.toUtf8(), m_node->dynamicProperty(property));
        } else {
            setProperty(property.toUtf8(), QVariant::Int);
        }
    }
}

#include "moc_nodewrapper.cpp"
