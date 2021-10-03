/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef DOCUMENTWRAPPER_H
#define DOCUMENTWRAPPER_H

#include "graphtheory_export.h"
#include "kernel.h"
#include "typenames.h"
#include "node.h"
#include "edge.h"
#include "graphdocument.h"

#include <QScriptEngine>
#include <QObject>
#include <QColor>

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
    DocumentWrapper(GraphDocumentPtr document, QScriptEngine *engine);
    ~DocumentWrapper() override;

    QScriptEngine * engine() const;

    /**
     * \return wrapper for \p node
     */
    NodeWrapper * nodeWrapper(NodePtr node) const;

    /**
     * \return wrapper for \p edge
     */
    EdgeWrapper * edgeWrapper(EdgePtr edge) const;

    Q_INVOKABLE QScriptValue node(int id) const;
    Q_INVOKABLE QScriptValue nodes() const;
    Q_INVOKABLE QScriptValue nodes(int type) const;
    Q_INVOKABLE QScriptValue edges() const;
    Q_INVOKABLE QScriptValue edges(int type) const;
    Q_INVOKABLE QScriptValue createNode(int x, int y);
    Q_INVOKABLE QScriptValue createEdge(GraphTheory::NodeWrapper *from, GraphTheory::NodeWrapper *to);
    Q_INVOKABLE void remove(GraphTheory::NodeWrapper *node);
    Q_INVOKABLE void remove(GraphTheory::EdgeWrapper *edge);

Q_SIGNALS:
    void message(const QString &messageString, Kernel::MessageType type) const;

private Q_SLOTS:
    void registerWrapper(NodePtr node);
    void registerWrapper(EdgePtr edge);

private:
    Q_DISABLE_COPY(DocumentWrapper)
    const GraphDocumentPtr m_document;
    QScriptEngine *m_engine;
    QMap<NodePtr, NodeWrapper*> m_nodeMap;
    QMap<EdgePtr, EdgeWrapper*> m_edgeMap;
};
}

#endif
