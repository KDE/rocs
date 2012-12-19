/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#define PI_ 3.14159265358979323846

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

    enum GraphType {
        MESH,
        STAR,
        CIRCLE,
        RANDOM,
        ER_RANDOM,
        RANDOM_TREE
    };

public:
    explicit GenerateGraphWidget(Document* graphDoc, QWidget *parent = 0);
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

    /**
     * generates a random tree graph given the following parameters
     * \param   int nodes   number of nodes
     * \param   int seed    random seed for random number generator
     */
    void generateRandomTreeGraph(int nodes, int seed);
    Document* graphDoc_;
    int selectedGraphType_;
    QGridLayout *gridLayout_;
    QWidget* graphOptionsWidget_;

    Ui::GenerateGraphWidget *ui;
};

#endif // GENERATEGRAPHWIDGET_H

