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

#include "documentwrapper.h"
#include "nodewrapper.h"
#include "edgewrapper.h"
#include "graphdocument.h"
#include "nodetype.h"
#include "edge.h"

#include <QDebug>

using namespace GraphTheory;

DocumentWrapper::DocumentWrapper(GraphDocumentPtr document, QScriptEngine *engine)
    : m_document(document)
    , m_engine(engine)
{

}

DocumentWrapper::~DocumentWrapper()
{

}

QScriptValue DocumentWrapper::nodes()
{
    QScriptValue array = m_engine->newArray();
    QList<NodeWrapper*> nodeWrappers;
    foreach(NodePtr node, m_document->nodes()) {
        if (m_nodeMap.contains(node)) {
            nodeWrappers.append(m_nodeMap.value(node));
        } else {
            NodeWrapper *wrapper = new NodeWrapper(node, this);
            m_nodeMap.insert(node, wrapper);
            nodeWrappers.append(wrapper);
        }
    }
    return m_engine->toScriptValue(nodeWrappers);
}

QScriptValue DocumentWrapper::edges()
{
    QScriptValue array = m_engine->newArray();
    QList<EdgeWrapper*> edgeWrappers;
    foreach(EdgePtr edge, m_document->edges()) {
        if (m_edgeMap.contains(edge)) {
            edgeWrappers.append(m_edgeMap.value(edge));
        } else {
            EdgeWrapper *wrapper = new EdgeWrapper(edge, this);
            m_edgeMap.insert(edge, wrapper);
            edgeWrappers.append(wrapper);
        }
    }
    return m_engine->toScriptValue(edgeWrappers);
}
