/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GRAPHSTRUCTURE_H
#define GRAPHSTRUCTURE_H

#include "DataStructure.h"

namespace Rocs{
class ROCSLIB_EXPORT GraphStructure : public DataStructure {
    Q_OBJECT
    public:
        typedef enum {
            UNDIRECTED,
            DIRECTED,
            MULTIGRAPH
        } GRAPH_TYPE;

      //to avoid hide some methods
    using DataStructure::remove;
    using DataStructure::addPointer;
    using DataStructure::addData;

    static DataStructurePtr create(Document *parent);
    static DataStructurePtr create(DataStructurePtr other, Document *parent);

    GraphStructure ( Document* parent = 0 );
    ~GraphStructure();
    void importStructure(DataStructurePtr other);

    PointerPtr addPointer(DataPtr from, DataPtr to, int pointerType=0);
    DataPtr addData(QString name, int dataType=0);
    
    public slots:
        /**
         * returns a list of all nodes of the graph as array
         */
        QScriptValue list_nodes();
        
        /**
         * returns a list of all edges of the graph as array
         */
        QScriptValue list_edges();
        
        QScriptValue add_node(const QString& name);
        QScriptValue add_edge(Data* fromRaw, Data* toRaw);
        QScriptValue node_byname(const QString& name);
        
        /**
         * Computes the Dijkstra's shortest path algorithm to compute
         * the shortes path from "from" to "to". Note: this shortest path
         * algorithm works only for graphs with all edges values non-negative.
         * For undirected graphs reverse edges are add automatically.
         * The algorithm has time complexity O(V log V + E).
         * 
         * \param from the node from which the computation starts
         * \param to the node to which the shortest path shall be computed
         * \return the edge set of the shortest path
         */
        QScriptValue dijkstra_shortest_path(Data* fromRaw, Data* toRaw);
        
        bool directed();
        GRAPH_TYPE graphType();
        void setGraphType(int type);

    private:
        GraphStructure::GRAPH_TYPE _type;
};
}
#endif // GRAPHSTRUCTURE_H
