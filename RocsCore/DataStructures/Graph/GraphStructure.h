/*
    This file is part of Rocs.
    Copyright 2011       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2011       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "rocs_graphstructure_export.h"
#include "DataStructure.h"

namespace Rocs
{
class ROCS_GRAPHSTRUCTURE_EXPORT GraphStructure : public DataStructure
{
    Q_OBJECT
public:
    typedef enum {
        Graph,
        Multigraph
    } GRAPH_TYPE;

    //to avoid hide some methods
    using DataStructure::remove;
    using DataStructure::addPointer;
    using DataStructure::addData;

    static DataStructurePtr create(Document *parent);
    static DataStructurePtr create(DataStructurePtr other, Document *parent);

    explicit GraphStructure(Document* parent = 0);
    ~GraphStructure();
    void importStructure(DataStructurePtr other);

    /**
     * Internal method to create new graph edge.
     * Use \see Pointer::create(...) for creating new edges.
     * \param from is the origin of the new edge
     * \param to is the target of the new edge
     * \param pointerType is the type of this edge, defaults to 0
     * \return the created edge as PointerPtr
     */
    PointerPtr addPointer(DataPtr from, DataPtr to, int pointerType);

    /**
     * Internal method to create new graph node.
     * Use \see Data::create(...) for creating new nodes.
     * \param name is the name of the node
     * \param dataType is the type of this node, defaults to 0
     * \return the created node as DataPtr
     */
    DataPtr addData(const QString& name, int dataType);

    /**
     * Returns type of the graph given by enum \see GRAPH_TYPE.
     * \return graph type
     */
    GRAPH_TYPE graphType() const;

    bool multigraph() const;

    /**
     * Gives a map with plugin specific properties of the data structure.
     * Implementation of virtual method \see DataStructure::pluginProperties()
     * \return map keys are property names, values are property values.
     */
    QMap<QString, QString> pluginProperties() const;

    /**
     * Set plugin specific properties of data structure.
     * \param identifier is the unique identifier for this property
     * \param value is the to be set value for the property
     */
    void setPluginProperty(const QString& identifier, const QString& property);

    /**
     * Computes the Dijkstra's shortest path algorithm to compute
     * all shortest path distances from \p from. Note: this shortest path
     * algorithm works only for graphs with all edges values non-negative.
     * For undirected graphs reverse edges are add automatically.
     * The algorithm has time complexity O(V log V + E).
     *
     * \param from the node from which the computation starts
     * \return list is map with keys = target elements, values = path to target
     */
    QMap<DataPtr,PointerList> dijkstraShortestPaths(DataPtr from);

public slots:
    /**
     * Setter for graph type. No conversations are performed.
     * \param type as specified  by enum GRAPH_TYPE
     * \return void
     */
    void setGraphType(int type);

    /**
     * Returns a list of all nodes of the graph.
     * \return array containing the nodes
     */
    QScriptValue list_nodes();

    /**
     * Returns a list of all nodes of given \p type in the graph.
     * \param type is the overlay for the to created pointer
     * \return array containing the nodes
     */
    QScriptValue list_nodes(int type);

    /**
     * Returns a list of all edges of the graph.
     * \return array containing the edges
     */
    QScriptValue list_edges();

    /**
     * Returns a list of all edges of given \p type in the graph.
     * \param type is the overlay for the to created pointer
     * \return array containing the edges
     */
    QScriptValue list_edges(int type);

    /**
     * Gives array of edges of specified overlay. If overlay
     * does not exist an empty array is returned.
     * \param overlay integer that identifies the overlay
     * \return QScriptValue array
     */
    QScriptValue overlay_edges(int overlay);

    /**
     * Creates a new node with specified \param name.
     * \param name of the new node
     * \return script value for the new node
     */
    QScriptValue add_node(const QString& name);

    /**
     * Creates a new overlay edge from \param fromRaw to \param toRaw
     * at overlay \param overlay. If the overlay does not exist no pointer
     * is created.
     * \param fromRaw is origin of pointer
     * \param toRaw is target of pointer
     * \param overlay is the overlay for the to created pointer
     * \return script value for the new pointer
     */
    QScriptValue add_overlay_edge(Data* fromRaw, Data* toRaw, int overlay);

    /**
     * Creates a new edge from \param fromRaw to \param toRaw.
     * \param fromRaw is origin of pointer
     * \param toRaw is target of pointer
     * \return script value for the new pointer
     */
    QScriptValue add_edge(Data* fromRaw, Data* toRaw);

    /**
     * Computes the Dijkstra's shortest path algorithm to compute
     * the shortes path from \param from to \param to. Note: this shortest path
     * algorithm works only for graphs with all edges values non-negative.
     * For undirected graphs reverse edges are add automatically.
     * The algorithm has time complexity O(V log V + E).
     *
     * \param from the node from which the computation starts
     * \param to the node to which the shortest path shall be computed
     * \return the edge set of the shortest path
     */
    QScriptValue dijkstra_shortest_path(Data* fromRaw, Data* toRaw);

    /**
     * Computes the Dijkstra's shortest path algorithm to compute
     * all shortest path distances from \p from. If edge has value 0, the edge value
     * is set to 1. Note: this shortest path algorithm works only for graphs with all
     * edges values non-negative. For undirected graphs reverse edges are add automatically.
     * The algorithm has time complexity O(V log V + E).
     *
     * \param from the node from which the computation starts
     * \return the edge set of the shortest path
     */
    QScriptValue distances(Data* fromRaw);

private:
    GraphStructure::GRAPH_TYPE _type;
};
}
#endif // GRAPHSTRUCTURE_H
