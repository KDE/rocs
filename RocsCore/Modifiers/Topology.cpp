/*
    This file is part of Rocs.
    Copyright (C) 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include "Topology.h"

#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"

#include <QList>
#include <QPair>
#include <QVector>

#include <KDebug>

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>

#include "CoreTypes.h"

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
typedef QPair<int, int> Edge;


Topology::Topology()
{

}

Topology::~Topology()
{

}

void Topology::applyMinCutTreeAlignment(DataList dataList)
{
    // dataList must be at least of length 2, and two nodes cannot have crossing edges
    if (dataList.count() < 3) {
        return;
    }

    PositionVec position_vec(dataList.count());

    // set box inside which we may reposition
    QList<qreal> xList;
    QList<qreal> yList;
    foreach(DataPtr data, dataList) {
        xList << data->x();
        yList << data->y();
    }
    qSort(xList.begin(), xList.end());
    qSort(yList.begin(), yList.end());

    // do not perform algorithm if graph is very dense:
    // this prevents very long algorithm computations and possible threading issues
    if (xList.last() - xList.first() < 10 && yList.last() - yList.first() < 10 ) {
        qDebug() << "Aborting min cut alignment: nodes are already close to each other.";
        return;
    }

    topology_type topology(xList.first(), yList.first(), xList.last(), yList.last());

    // create IDs for all nodes
    QMap<Data*, int> node_mapping;
    QMap<QPair<int, int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataPtr data, dataList) {
        node_mapping[data.get()] = counter++;
    }

    DataStructurePtr ds = dataList.first()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach(PointerPtr p, ds->pointers()) {
        edges[counter++] = Edge(node_mapping[p->from().get()], node_mapping[p->to().get()]);
    }

    // setup the graph
    Graph graph(
        edges.begin(),
        edges.end(),
        dataList.count()
    );

    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, graph));
    counter = 0;
    foreach(DataPtr data, dataList) {
        positionMap[counter][0] = data->x();
        positionMap[counter][1] = data->y();
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
    foreach(DataPtr data, dataList) {
        Vertex v = boost::vertex(node_mapping[data.get()], graph);
        data->setX(positionMap[v][0]);
        data->setY(positionMap[v][1]);
    }
}

void Topology::applyCircleAlignment(DataList dataList, qreal radius)
{
    if (dataList.length() == 0) {
        return;
    }

    PositionVec position_vec(dataList.count());

    if(radius == 0) {
        // set box inside which we may reposition
        QList<qreal> xList;
        QList<qreal> yList;
        foreach(DataPtr data, dataList) {
            xList << data->x();
            yList << data->y();
        }
        qSort(xList.begin(), xList.end());
        qSort(yList.begin(), yList.end());

        radius = fmax(abs(xList.first() - xList.last()), abs(yList.first() - yList.last())) / 2;
    }

    // create IDs for all nodes
    QMap<Data*, int> node_mapping;
    QMap<QPair<int, int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataPtr data, dataList) {
        node_mapping[data.get()] = counter++;
    }

    DataStructurePtr ds = dataList.first()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach(PointerPtr p, ds->pointers()) {
        edges[counter++] = Edge(node_mapping[p->from().get()], node_mapping[p->to().get()]);
    }

    // setup the graph
    Graph graph(
        edges.begin(),
        edges.end(),
        dataList.count()
    );

    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, graph));
    counter = 0;
    foreach(DataPtr data, dataList) {
        positionMap[counter][0] = data->x();
        positionMap[counter][1] = data->y();
        counter++;
    }

    // layout to circle
    boost::circle_graph_layout< Graph, PositionMap >
    (graph,
     positionMap,
     radius
    );

    // put nodes at whiteboard as generated
    foreach(DataPtr data, dataList) {
        Vertex v = boost::vertex(node_mapping[data.get()], graph);
        data->setX(positionMap[v][0]);
        data->setY(positionMap[v][1]);
    }
}


void Topology::directedGraphDefaultTopology(DataStructurePtr dataStructure)
{
    //TODO: port to graphviz layout functions
    kDebug() << "Temporary implementation, should be replaced soon.";

    QList<DataPtr> allDataList;
    foreach(int type, dataStructure->document()->dataTypeList()) {
        allDataList << dataStructure->dataList(type);
    }
    applyCircleAlignment(allDataList, 300);
    applyMinCutTreeAlignment(allDataList);
}


void Topology::undirectedGraphDefaultTopology(DataStructurePtr dataStructure)
{
    //TODO: port to graphviz layout functions
    kDebug() << "Temporary implementation, should be replaced soon.";

    QList<DataPtr> allDataList;
    foreach(int type, dataStructure->document()->dataTypeList()) {
        allDataList << dataStructure->dataList(type);
    }
    applyCircleAlignment(allDataList, 300);
    applyMinCutTreeAlignment(allDataList);
}

