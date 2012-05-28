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

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <Data.h>

class GraphNode : public Data
{
    Q_OBJECT

public:
    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType);
    GraphNode(DataStructurePtr parent, int uniqueIdentifier, int dataType);

    virtual void setEngine(QScriptEngine* _engine);
    virtual ~GraphNode();
public slots:
    QScriptValue adj_nodes();
    QScriptValue adj_edges();
    QScriptValue input_edges();
    QScriptValue output_edges();
    QScriptValue loop_edges();
    QScriptValue connected_edges(Data* n);

    /**
     * Gives array of edges at specified overlay if present.
     * Otherwise if overlay does not exist returns empty array.
     * Function respects type of graph, i.e., returns either output edges or
     * output plus input edges if graph is directed or undirected, respectively.
     * \param overlay is identifier for overlay.
     */
    QScriptValue overlay_edges(int overlay);
};

#endif // GRAPHNODE_H
