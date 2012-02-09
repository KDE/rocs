/* 
    This file is part of Rocs,
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>

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

DataPtr GraphNode::create(DataStructurePtr parent, int uniqueIdentifier)
{
    return Data::create<GraphNode>(parent, uniqueIdentifier);
}


GraphNode::GraphNode(DataStructurePtr parent, int uniqueIdentifier): Data(parent, uniqueIdentifier)
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
    if (n==0)
        return QScriptValue();
    return Data::connected_pointers( n->getData() );
}

QScriptValue GraphNode::input_edges()
{
  return Data::input_pointers();
}

QScriptValue GraphNode::loop_edges()
{
  return Data::loop_pointers();
}

QScriptValue GraphNode::output_edges()
{
  return Data::output_pointers();
}

