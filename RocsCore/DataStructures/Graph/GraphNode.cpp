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

QScriptValue GraphNode::adj_nodes()
{
    return Data::adj_data();
}

QScriptValue GraphNode::adj_edges()
{
    return Data::adj_pointers();
}

QScriptValue GraphNode::connected_edges(Data* n)
{
    if (n == 0) {
        return QScriptValue();
    }
    return Data::connected_pointers(n);
}

QScriptValue GraphNode::input_edges()
{
    return Data::input_pointers();
}

QScriptValue GraphNode::output_edges()
{
    return Data::output_pointers();
}

QScriptValue GraphNode::overlay_edges(int overlay)
{
    Data::output_pointers();
    boost::shared_ptr<Rocs::GraphStructure> ds = boost::static_pointer_cast<Rocs::GraphStructure>(dataStructure());
    return Data::output_pointers(overlay);
}
