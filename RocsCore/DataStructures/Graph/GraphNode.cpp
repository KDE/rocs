/*
    This file is part of Rocs,
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GraphNode.h"
#include "GraphStructure.h"
#include "Pointer.h"

#include <KLocale>

DataPtr GraphNode::create(DataStructurePtr parent, int uniqueIdentifier, int dataType)
{
    return Data::create<GraphNode>(parent, uniqueIdentifier, dataType);
}

GraphNode::GraphNode(DataStructurePtr parent, int uniqueIdentifier, int dataType) :
    Data(parent, uniqueIdentifier, dataType)
{
}

void GraphNode::setEngine(QScriptEngine* _engine)
{
    Data::setEngine(_engine);
}

GraphNode::~GraphNode()
{
}

QScriptValue GraphNode::edges()
{
    return edges(0);
}

QScriptValue GraphNode::edges(int type)
{
    return Data::adj_pointers(type);
}

QScriptValue GraphNode::inEdges()
{
    return Data::input_pointers();
}

QScriptValue GraphNode::outEdges()
{
    return outEdges(0);
}

QScriptValue GraphNode::outEdges(int type)
{
    return Data::output_pointers(type);
}

QScriptValue GraphNode::inEdges(int type)
{
    return Data::input_pointers(type);
}

QScriptValue GraphNode::neighbors()
{
    //TODO ensure that direction of edges is respected (this should already be the case but not unit-tested!
    return Data::adj_data();
}

QScriptValue GraphNode::edgesTo(Data* node)
{
    if (node == 0) {
        return QScriptValue();
    }
    return Data::connected_pointers(node);
}

QScriptValue GraphNode::adj_nodes()
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("adj_nodes()"),
        QString("neighbors()")));
    return Data::adj_data();
}

QScriptValue GraphNode::adj_edges()
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("adj_edges()"),
        QString("edges()")));
    return Data::adj_pointers();
}

QScriptValue GraphNode::connected_edges(Data* n)
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("connected_edges(node)"),
        QString("edgesTo(node)")));
    if (n == 0) {
        return QScriptValue();
    }
    return Data::connected_pointers(n);
}

QScriptValue GraphNode::input_edges()
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("input_edges()"),
        QString("inEdges()")));
    return Data::input_pointers();
}

QScriptValue GraphNode::output_edges()
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("output_edges()"),
        QString("outEdges()")));
    return Data::output_pointers();
}

QScriptValue GraphNode::overlay_edges(int overlay)
{
    emit scriptError(i18n("The method \"%1\" is deprecated, please use \"%2\" instead.",
        QString("edges(overlay)"),
        QString("edges(type)")));
    return Data::output_pointers(overlay);
}
