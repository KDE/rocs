/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef GENERATEGRAPHWIDGET_H
#define GENERATEGRAPHWIDGET_H

#include <QWidget>
#include <KDialog>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>

class QGridLayout;
class Document;

namespace Ui
{
class GenerateGraphWidget;
}

class GenerateGraphWidget :
    public KDialog
{
    Q_OBJECT

    typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
            boost::property<boost::vertex_name_t, std::string> >
            Graph;

    typedef boost::rectangle_topology<> topology_type;

    typedef topology_type::point_type point_type;

    typedef std::vector<point_type> PositionVec;

    typedef boost::iterator_property_map < PositionVec::iterator,
            boost::property_map<Graph, boost::vertex_index_t>::type >
            PositionMap;

    enum GraphType {
        MESH,
        STAR,
        CIRCLE,
        RANDOM,
        ER_RANDOM
    };

public:
    GenerateGraphWidget(Document* graphDoc, QWidget *parent = 0);
    ~GenerateGraphWidget();

public slots:
    void setGraphType(int type);
    void generateGraph();

private:
    void generateCircle(int numberSatelliteNodes);
    void generateMesh(int rows, int columns);

    /**
     * generates a random graph given by following parameters
     * \param   int nodes   number of nodes
     * \param   int edges   number of randomly assigned edges
     * \param   int seed    random seed for random number generator
     * \param   bool selfEdges if true self edges are generated, otherwise not
     */
    void generateRandomGraph(int nodes, int randomEdges, int seed, bool selfEdges);

    /**
     * generates graph with one center an the specified number of satellite nodes
     * \param   int numberNodes   number of satellite nodes
     */
    void generateStar(int numberNodes);

    /**
     * generates a Erdös-Renyi random graph given by following parameters
     * \param   int nodes   number of nodes
     * \param   int edgeProbability   between each two nodes add edge with this probability
     * \param   int seed    random seed for random number generator
     * \param   bool selfEdges if true self edges are generated, otherwise not
     */
    void generateErdosRenyiRandomGraph(int nodes, double edgeProbability, int seed, bool selfEdges);

    Document* graphDoc_;
    static const double PI_ = 3.14159265358979323846;
    int selectedGraphType_;
    QGridLayout *gridLayout_;
    QWidget* graphOptionsWidget_;

    Ui::GenerateGraphWidget *ui;
};

#endif // GENERATEGRAPHWIDGET_H

