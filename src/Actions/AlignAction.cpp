/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>
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

#include "AlignAction.h"
#include <KIcon>
#include "GraphVisualEditor.h"
#include "GraphScene.h"
#include "generics.h"
#include "DataStructure.h"
#include "Pointer.h"
#include <KDebug>


#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random/linear_congruential.hpp>
#include <QPair>

namespace boost {
void throw_exception( std::exception const & ) {} } // do noop on exception

AlignAction::AlignAction(const QString& tooltip,AlignAction::Orientation o, QWidget *parent)
        : KAction(KIcon(), tooltip, parent) {
    m_orientation = o;
    connect(this, SIGNAL(triggered()), this, SLOT(align()));
    switch (o) {
    case Left :
        setIcon(KIcon("rocsalignleft"));
        break;
    case Right :
        setIcon(KIcon("rocsalignright"));
        break;
    case Top :
        setIcon(KIcon("rocsaligntop"));
        break;
    case Bottom :
        setIcon(KIcon("rocsalignbottom"));
        break;
    case HCenter :
        setIcon(KIcon("rocsalignhmiddle"));
        break;
    case VCenter :
        setIcon(KIcon("rocsalignvmiddle"));
        break;
    case Circle :
        setIcon(KIcon("rocsaligncircle"));
        break;
    case MinCutTree :
        setIcon(KIcon("rocsaligntree"));
        break;
    }
}

void AlignAction::align() {
    GraphVisualEditor *gEditor = qobject_cast<GraphVisualEditor*>(parent());
    QList<DataItem*> l = gEditor->selectedNodes();

    if (l.size() < 1) return;
    gEditor->scene()->setHideEdges(true);
    switch (m_orientation) {
    case Left :
    case VCenter :
        qSort(l.begin(), l.end(),  leftLessThan);
        alignX(l);
        break;
    case Bottom :
    case HCenter :
        qSort(l.begin(), l.end(), bottomLessThan);
        alignY(l);
        break;
    case Right :
        qSort(l.begin(), l.end(), rightLessThan);
        alignX(l);
        break;
    case Top :
        qSort( l.begin(), l.end(), topLessThan);
        alignY(l);
        break;
    case Circle :
        alignCircle(l);
        break;
    case MinCutTree :
        alignMinCutTree(l);
        break;
    }

    gEditor->scene()->setHideEdges(false);
}

void AlignAction::alignY(QList<DataItem*>& dataItemList) {
    qreal final = dataItemList[0]->data()->y();

    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = dataItemList[dataItemList.size()-1]->data()->y();
        final = (final + otherSide) / 2;
    }
    foreach(DataItem *i, dataItemList) {
        i->data()->setY(final);
    }
}

void AlignAction::alignX(QList<DataItem*>& dataItemList) {
    qreal final = dataItemList[0]->data()->x();
    if (m_orientation == VCenter || m_orientation == HCenter) {
        qreal otherSide = dataItemList[dataItemList.size()-1]->data()->x();
        final = (final + otherSide) / 2;
    }
    foreach(DataItem *i, dataItemList) {
        i->data()->setX(final);
    }
}

void AlignAction::alignCircle(QList< DataItem* >& dataItemList) {
    QList<qreal> xList;
    QList<qreal> yList;
    QPoint center = QPoint(0,0);
    foreach(DataItem *i, dataItemList) {
        xList << i->data()->x();
        yList << i->data()->y();
        center.setX(center.x() + i->data()->x());
        center.setY(center.y() + i->data()->y());
    }
    center.setX( center.x()/xList.length() );
    center.setY( center.y()/yList.length() );
    qSort(xList.begin(), xList.end());
    qSort(yList.begin(), yList.end());

    qreal radius = fmax(abs(xList.first()-xList.last()),abs(yList.first()-yList.last()))/2;

    foreach(DataItem *i, dataItemList) {
        qreal centerDistance = sqrt( pow(i->data()->x()-center.x(),2) + pow(i->data()->y()-center.y(),2) );
        i->data()->setX(center.x() + (i->data()->x() - center.x())/centerDistance*radius);
        i->data()->setY(center.y() + (i->data()->y() - center.y())/centerDistance*radius);
    }
}

void AlignAction::alignMinCutTree(QList< DataItem* >& dataItemList) {
    typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS,
        boost::property<boost::vertex_name_t, std::string> >
        Graph;
    typedef boost::rectangle_topology<> topology_type;
    typedef topology_type::point_type point_type;
    typedef QVector<point_type> PositionVec;
    typedef boost::iterator_property_map<PositionVec::iterator,
        boost::property_map<Graph, boost::vertex_index_t>::type> 
        PositionMap;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef QPair<int, int> Edge;

    PositionVec position_vec(dataItemList.count());

    // set box inside which we may reposition
    QList<qreal> xList;
    QList<qreal> yList;
    foreach(DataItem *i, dataItemList) {
        xList << i->data()->x();
        yList << i->data()->y();
    }
    qSort(xList.begin(), xList.end());
    qSort(yList.begin(), yList.end());
    topology_type topology( xList.first(), yList.first(), xList.last(), yList.last());

    // create IDs for all nodes
    QMap<Data*,int> node_mapping;
    QMap<QPair<int,int>, PointerPtr > edge_mapping; // to map all edges back afterwards
    int counter = 0;
    foreach(DataItem *data, dataItemList) {
        node_mapping[data->data().get()] = counter++;
    }

    DataStructurePtr ds = dataItemList.first()->data()->dataStructure();
    QVector<Edge> edges(ds->pointers().count());

    counter = 0;
    foreach( PointerPtr p, ds->pointers() ){
        edges[counter++] = Edge(node_mapping[p->from().get()], node_mapping[p->to().get()]);
    }

    // setup the graph
    Graph graph(
        edges.begin(),
        edges.end(),
        dataItemList.count()
    );

    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, graph));
    counter = 0;
    foreach(DataItem *data, dataItemList) {
        positionMap[counter][0] = data->data()->x();
        positionMap[counter][1] = data->data()->y();
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
    foreach(DataItem *data, dataItemList) {
        Vertex v = boost::vertex(node_mapping[data->data().get()], graph);
        data->data()->setX( positionMap[v][0] );
        data->data()->setY( positionMap[v][1] );
    }
}

