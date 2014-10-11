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

#ifndef DOCUMENTWRAPPER_H
#define DOCUMENTWRAPPER_H

#include "libgraphtheoryexport.h"
#include "typenames.h"
#include "node.h"
#include "edge.h"
#include "graphdocument.h"

#include <QtScript>
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
    virtual ~DocumentWrapper();

    /**
     * \return wrapper for \p node
     */
    NodeWrapper * nodeWrapper(NodePtr node) const;

    /**
     * \return wrapper for \p edge
     */
    EdgeWrapper * edgeWrapper(EdgePtr edge) const;

    Q_INVOKABLE QScriptValue nodes();
    Q_INVOKABLE QScriptValue edges();

Q_SIGNALS:

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
