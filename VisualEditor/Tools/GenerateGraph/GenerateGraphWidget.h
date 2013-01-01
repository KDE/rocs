/*
    This file is part of Rocs.
    Copyright (C) 2011-2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

class Document;

namespace Ui
{
class GenerateGraphWidget;
}

class GenerateGraphWidget
:   public KDialog
{
    Q_OBJECT

    enum GraphGenerator {
        MESH,
        STAR,
        CIRCLE,
        RANDOM,
        ER_RANDOM,
        RANDOM_TREE
    };

public:
    explicit GenerateGraphWidget(QWidget *parent = 0);
    ~GenerateGraphWidget();

public slots:
    /**
     * Select graph generator by its index.
     *
     * \param index the index of the graph generator
     */
    void setGraphGenerator(int index);

    /**
     * Set seed for the internal random number generator.
     *
     * \param seed is the random number generator seed.
     */
    void setSeed(int seed);

    /**
     * Set the type of data elements for the generator.
     *
     * \param type is the DataType ID
     */
    void setDataType(int type);

    /**
     * Set the type of pointers for the generator.
     *
     * \param type is the DataType ID
     */
    void setPointerType(int type);

    /**
     * Set the unique graph identifier for the next generated graph.
     * If \p identifier is already in use, the lexicographically next identifier will be used.
     *
     * \param identifier for the next generated graph
     */
    void setGraphIdentifier(const QString &identifier);

    /**
     * Generate the graph with the previously set configuration.
     */
    void generateGraph();

private:
    /**
     * Generate circle graph with specified number of nodes.
     *
     * \param nodes is the number of nodes of the generated graph
     */
    void generateCircle(int nodes);

    /**
     * Generate mesh graph with specified number of \p rows and \p columns.
     *
     * \param rows is the number of rows of the generated graph
     * \param columns is the number of columns of the generated graph
     */
    void generateMesh(int rows, int columns);

    /**
     * Generate a random graph by assigning uniformly at random \p edges many pointers to the set of nodes.
     *
     * \param nodes is the number of nodes of the generated graph
     * \param edges is the number of edges of the generated graph
     * \param seed is the seed for random number generator
     * \param selfEdges if true self edges are generated, otherwise not
     */
    void generateRandomGraph(int nodes, int edges, bool selfEdges);

    /**
     * Generate a star graph with specified number of nodes.
     *
     * \param satelliteNodes is the number of satellite nodes of the generated graph
     */
    void generateStar(int satelliteNodes);

    /**
     * Generate an Erdös-Renyi random graph. This graph is created by first creating a set of \p nodes
     * many data elemnts and then creating any possibly edge with probability \p edgeProbability.
     *
     * \param nodes is the number of nodes of the generated graph
     * \param edgeProbability is the probability for creating an arbitrary edge
     * \param seed is the seed for random number generator
     * \param selfEdges if true self edges are generated, otherwise not
     */
    void generateErdosRenyiRandomGraph(int nodes, double edgeProbability, bool selfEdges);

    /**
     * Generate a random tree by iterating the following process: First create a root node and than
     * for (\p nodes - 1) rounds create for a node (selected uniformly at random) a child.
     *
     * \param nodes is number of nodes
     * \param seed is the seed for random number generator
     */
    void generateRandomTreeGraph(int nodes);


    int seed_;
    int dataType_;
    int pointerType_;
    QString identifier_;
    GraphGenerator graphGenerator_;
    Ui::GenerateGraphWidget *ui;

    QList<QString> defaultIdentifiers;
};

#endif // GENERATEGRAPHWIDGET_H

