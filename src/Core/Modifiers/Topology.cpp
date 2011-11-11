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

#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>

#include "Rocs_Typedefs.h"

Topology::Topology()
{

}

Topology::~Topology()
{

}

void Topology::applyMinCutTreeAlignment(DataList dataList) 
{
    // dataList must be at least of length 2, and two nodes cannot have crossing edges
    if (dataList.count() < 3)
        return;

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
    topology_type topology( xList.first(), yList.first(), xList.last(), yList.last());

    // create IDs for all nodes
    QMap<Data*,int> node_mapping;
    QMap<QPair<int,int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataPtr data, dataList) {
        node_mapping[data.get()] = counter++;
    }

    DataStructurePtr ds = dataList.first()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach( PointerPtr p, ds->pointers() ){
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
    (   graph,
        positionMap,
        topology,
        boost::cooling(boost::linear_cooling<double>(100))
    );

    // put nodes at whiteboard as generated
    foreach(DataPtr data, dataList) {
        Vertex v = boost::vertex(node_mapping[data.get()], graph);
        data->setX( positionMap[v][0] );
        data->setY( positionMap[v][1] );
    }
}

void Topology::applyCircleAlignment(DataList dataList) {
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
    
    qreal radius = fmax(abs(xList.first()-xList.last()),abs(yList.first()-yList.last()))/2;

    // create IDs for all nodes
    QMap<Data*,int> node_mapping;
    QMap<QPair<int,int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataPtr data, dataList) {
        node_mapping[data.get()] = counter++;
    }

    DataStructurePtr ds = dataList.first()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach( PointerPtr p, ds->pointers() ){
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
    (   graph,
        positionMap,
        radius
    );

    // put nodes at whiteboard as generated
    foreach(DataPtr data, dataList) {
        Vertex v = boost::vertex(node_mapping[data.get()], graph);
        data->setX( positionMap[v][0] );
        data->setY( positionMap[v][1] );
    }
}

