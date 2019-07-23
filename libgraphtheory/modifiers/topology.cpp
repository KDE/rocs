/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "topology.h"
#include "graphdocument.h"
#include "edge.h"
#include "logging_p.h"

#include <QList>
#include <QPair>
#include <QVector>

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>

using namespace GraphTheory;

typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
        boost::property<boost::vertex_name_t, std::string> >
        Graph;
typedef boost::rectangle_topology<> topology_type;
typedef topology_type::point_type point_type;
typedef QVector<point_type> PositionVec;
typedef boost::iterator_property_map < PositionVec::iterator,
        boost::property_map<Graph, boost::vertex_index_t>::type >
        PositionMap;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef QPair<int, int> BoostEdge;

// handle boost exceptions
namespace boost {
    void throw_exception(std::exception const &e) {
        qCCritical(GRAPHTHEORY_GENERAL) << "Exception:" << e.what();
    }
}

void Topology::applyMinCutTreeAlignment(NodeList nodes)
{
    // nodes must be at least of length 2, and two nodes cannot have crossing edges
    if (nodes.count() < 3) {
        return;
    }

    PositionVec position_vec(nodes.count());

    // set box inside which we may reposition
    QList<qreal> xList;
    QList<qreal> yList;
    foreach(NodePtr node, nodes) {
        xList << node->x();
        yList << node->y();
    }
    qSort(xList.begin(), xList.end());
    qSort(yList.begin(), yList.end());

    // do not perform algorithm if graph is very dense:
    // this prevents very long algorithm computations and possible threading issues
    if (xList.last() - xList.first() < 10 && yList.last() - yList.first() < 10 ) {
        qCDebug(GRAPHTHEORY_GENERAL) << "Aborting min cut alignment: nodes are already close to each other.";
        return;
    }

    topology_type topology(xList.first(), yList.first(), xList.last(), yList.last());

    // create IDs for all nodes
    QMap<NodePtr, int> node_mapping;
    QMap<QPair<int, int>, EdgePtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(NodePtr node, nodes) {
        node_mapping[node] = counter++;
    }

    QVector<BoostEdge> edges(nodes.first()->document()->edges().count());

    counter = 0;
    foreach(EdgePtr edge, nodes.first()->document()->edges()) {
        edges[counter++] = BoostEdge(node_mapping[edge->from()], node_mapping[edge->to()]);
    }

    // setup the graph
    Graph graph(
        edges.begin(),
        edges.end(),
        nodes.count()
    );

    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, graph));
    counter = 0;
    foreach(NodePtr node, nodes) {
        positionMap[counter][0] = node->x();
        positionMap[counter][1] = node->y();
        counter++;
    }

    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< topology_type, Graph, PositionMap >
    (graph,
     positionMap,
     topology,
     boost::cooling(boost::linear_cooling<double>(100))
    );

    // put nodes at whiteboard as generated
    foreach(NodePtr node, nodes) {
        Vertex v = boost::vertex(node_mapping[node], graph);
        node->setX(positionMap[v][0]);
        node->setY(positionMap[v][1]);
    }
}

void Topology::applyCircleAlignment(NodeList nodes, qreal radius)
{
    if (nodes.length() == 0) {
        return;
    }

    PositionVec position_vec(nodes.count());

    if(radius == 0) {
        // set box inside which we may reposition
        QList<qreal> xList;
        QList<qreal> yList;
        foreach(NodePtr node, nodes) {
            xList << node->x();
            yList << node->y();
        }
        qSort(xList.begin(), xList.end());
        qSort(yList.begin(), yList.end());

        radius = fmax(fabs(xList.first() - xList.last()), fabs(yList.first() - yList.last())) / 2;
    }

    // create IDs for all nodes
    QMap<NodePtr, int> node_mapping;
    QMap<QPair<int, int>, EdgePtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(NodePtr node, nodes) {
        node_mapping[node] = counter++;
    }

    QVector<BoostEdge> edges(nodes.first()->document()->edges().count());

    counter = 0;
    foreach(EdgePtr edge, nodes.first()->document()->edges()) {
        edges[counter++] = BoostEdge(node_mapping[edge->from()], node_mapping[edge->to()]);
    }

    // setup the graph
    Graph graph(
        edges.begin(),
        edges.end(),
        nodes.count()
    );

    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, graph));
    counter = 0;
    foreach(NodePtr node, nodes) {
        positionMap[counter][0] = node->x();
        positionMap[counter][1] = node->y();
        counter++;
    }

    // layout to circle
    boost::circle_graph_layout<Graph, PositionMap>( graph,
                                                    positionMap,
                                                    radius);

    // put nodes at whiteboard as generated
    foreach(NodePtr node, nodes) {
        Vertex v = boost::vertex(node_mapping[node], graph);
        node->setX(positionMap[v][0]);
        node->setY(positionMap[v][1]);
    }
}


void Topology::directedGraphDefaultTopology(GraphDocumentPtr document)
{
    //TODO: port to graphviz layout functions
    qCDebug(GRAPHTHEORY_GENERAL) << "Temporary implementation, should be replaced soon.";
    applyCircleAlignment(document->nodes(), 300);
    applyMinCutTreeAlignment(document->nodes());
}


void Topology::undirectedGraphDefaultTopology(GraphDocumentPtr document)
{
    //TODO: port to graphviz layout functions
    qCDebug(GRAPHTHEORY_GENERAL) << "Temporary implementation, should be replaced soon.";
    applyCircleAlignment(document->nodes(), 300);
    applyMinCutTreeAlignment(document->nodes());
}
