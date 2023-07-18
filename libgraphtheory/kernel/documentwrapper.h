/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef DOCUMENTWRAPPER_H
#define DOCUMENTWRAPPER_H

#include "edge.h"
#include "graphdocument.h"
#include "graphtheory_export.h"
#include "kernel.h"
#include "node.h"
#include "typenames.h"

#include <QColor>
#include <QObject>
#include <QJSEngine>

namespace GraphTheory
{
class DocumentWrapperPrivate;
class NodeWrapper;
class EdgeWrapper;

/**
 * \class DocumentWrapper
 * Wraps DocumentPtr to be accessible via QtScript.
 */
class DocumentWrapper : public QObject
{
    Q_OBJECT

public:
    DocumentWrapper(GraphDocumentPtr document, QJSEngine *engine);
    ~DocumentWrapper() override;

    QJSEngine *engine() const;

    /**
     * \return wrapper for \p node
     */
    NodeWrapper *nodeWrapper(NodePtr node) const;

    /**
     * \return wrapper for \p edge
     */
    EdgeWrapper *edgeWrapper(EdgePtr edge) const;

    Q_INVOKABLE QJSValue node(int id) const;
    Q_INVOKABLE QJSValue nodes() const;
    Q_INVOKABLE QJSValue nodes(int type) const;
    Q_INVOKABLE QJSValue edges() const;
    Q_INVOKABLE QJSValue edges(int type) const;
    Q_INVOKABLE QJSValue createNode(int x, int y);
    Q_INVOKABLE QJSValue createEdge(GraphTheory::NodeWrapper *from, GraphTheory::NodeWrapper *to);
    Q_INVOKABLE void remove(const QJSValue &object);

Q_SIGNALS:
    void message(const QString &messageString, Kernel::MessageType type) const;

private Q_SLOTS:
    void registerWrapper(NodePtr node);
    void registerWrapper(EdgePtr edge);

private:
    Q_DISABLE_COPY(DocumentWrapper)
    const GraphDocumentPtr m_document;
    QJSEngine *m_engine;
    QMap<NodePtr, NodeWrapper *> m_nodeMap;
    QMap<EdgePtr, EdgeWrapper *> m_edgeMap;
};
}

#endif
