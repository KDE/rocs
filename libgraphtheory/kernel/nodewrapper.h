/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef NODEWRAPPER_H
#define NODEWRAPPER_H

#include "documentwrapper.h"
#include "kerneltypes.h"
#include "node.h"
#include "typenames.h"
#include <QColor>
#include <QObject>

class QEvent;

namespace GraphTheory
{
class NodeWrapperPrivate;

/**
 * \class NodeWrapper
 * Wraps NodePtr to be accessible via QtScript. All properties of the node object are available
 * as common QObject properties.
 */
class NodeWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY positionChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY positionChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)

public:
    NodeWrapper(NodePtr node, DocumentWrapper *documentWrapper);

    ~NodeWrapper() override;

    NodePtr node() const;

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
    QString color() const;

    /**
     * set color of node to @c color
     */
    void setColor(const QString &colorString);

    /**
     * @return NodeType::id of corresponding node
     */
    int type() const;

    /**
     * Set NodeType of corresponding node by specifying its ID by @p typeId.
     * If @p typeId does not name the ID of any NodeType, the type is not changed.
     */
    void setType(int typeId);

    /**
     * @return list of all edges adjacent to this node
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> edges() const;

    /**
     * Method returns empty list if invalid or non-existing ID \p type is used.
     * @param type is the ID of an existing EdgeType
     * @return list of all edges adjacent to this node of type with ID \p type
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> edges(int type) const;

    /**
     * @return list of all edges that are either bidirectional or unidirectional and point to this node
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> inEdges() const;

    /**
     * Method returns empty list if invalid or non-existing ID \p type is used.
     * @param type is the ID of an existing EdgeType
     * @return list of all edges that are either bidirectional or unidirectional and point to this node
     *         with ID \p type
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> inEdges(int type) const;

    /**
     * @return list of all edges that are either bidirectional or unidirectional and point from this node
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> outEdges() const;

    /**
     * Method returns empty list if invalid or non-existing ID \p type is used.
     * @param type is the ID of an existing EdgeType
     * @return list of all edges that are either bidirectional or unidirectional and point from this node
     *          with ID \p type
     */
    Q_INVOKABLE QList<GraphTheory::EdgeWrapper *> outEdges(int type) const;

    /**
     * @return list of all neighbors connected to this node
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> neighbors() const;

    /**
     * @param type is the ID of an existing EdgeType
     * @return list of all neighbors connected to this node via an edge of type \p type
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> neighbors(int type) const;

    /**
     * @return list of all neighbors connected by an incoming edge
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> predecessors() const;

    /**
     * @param type is the ID of an existing EdgeType
     * @return list of all neighbors connected by an incoming edge of type \p type
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> predecessors(int type) const;

    /**
     * @return list of all neighbors connected by an outgoing edge
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> successors() const;

    /**
     * @param type is the ID of an existing EdgeType
     * @return list of all neighbors connected by an outgoing edge of type \p type
     */
    Q_INVOKABLE QList<GraphTheory::NodeWrapper *> successors(int type) const;

    /**
     * @return array of distances to given set of nodes
     */
    Q_INVOKABLE QJSValue distance(const QString &lengthProperty, QList<GraphTheory::NodeWrapper *> targets);

    /** reimplemented from QObject **/
    bool event(QEvent *e) override;

public Q_SLOTS:
    void updateDynamicProperties();

Q_SIGNALS:
    void message(const QString &messageString, GraphTheory::MessageType type) const;
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

Q_DECLARE_METATYPE(GraphTheory::NodeWrapper *)

#endif
