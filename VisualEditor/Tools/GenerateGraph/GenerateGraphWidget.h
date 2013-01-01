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
    explicit GenerateGraphWidget(Document *graphDoc, QWidget *parent = 0);
    ~GenerateGraphWidget();

public slots:
    void setGraphType(int type);
    void generateGraph();

private:
    /**
     * Generate circle graph with specified number of nodes.
     *
     * \param nodes is the number of nodes of the generated graph
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateCircle(int nodes, int pointerType, int dataType, const QString &identifier);

    /**
     * Generate mesh graph with specified number of \p rows and \p columns.
     *
     * \param rows is the number of rows of the generated graph
     * \param columns is the number of columns of the generated graph
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateMesh(int rows, int columns, int pointerType, int dataType, const QString &identifier);

    /**
     * Generate a random graph by assigning uniformly at random \p edges many pointers to the set of nodes.
     *
     * \param nodes is the number of nodes of the generated graph
     * \param edges is the number of edges of the generated graph
     * \param seed is the seed for random number generator
     * \param selfEdges if true self edges are generated, otherwise not
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateRandomGraph(int nodes, int edges, int seed, bool selfEdges, int pointerType, int dataType, const QString &identifier);

    /**
     * Generate a star graph with specified number of nodes.
     *
     * \param satelliteNodes is the number of satellite nodes of the generated graph
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateStar(int satelliteNodes, int pointerType, int dataType, const QString &identifier);

    /**
     * Generate an Erdös-Renyi random graph. This graph is created by first creating a set of \p nodes
     * many data elemnts and then creating any possibly edge with probability \p edgeProbability.
     *
     * \param nodes is the number of nodes of the generated graph
     * \param edgeProbability is the probability for creating an arbitrary edge
     * \param seed is the seed for random number generator
     * \param selfEdges if true self edges are generated, otherwise not
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateErdosRenyiRandomGraph(int nodes, double edgeProbability, int seed, bool selfEdges, int pointerType, int dataType, const QString &identifier);

    /**
     * Generate a random tree by iterating the following process: First create a root node and than
     * for (\p nodes - 1) rounds create for a node (selected uniformly at random) a child.
     *
     * \param nodes is number of nodes
     * \param seed is the seed for random number generator
     * \param pointerType is the type of the pointers of the generated graph
     * \param dataType is the type of the data elements of the generated graph
     * \param identifier is the identifier of the generated graph; it is expected to be unique within the current document
     */
    void generateRandomTreeGraph(int nodes, int seed, int pointerType, int dataType, const QString &identifier);

    Document *graphDoc_;
    int selectedGraphType_;
    QGridLayout *gridLayout_;
    QWidget *graphOptionsWidget_;

    Ui::GenerateGraphWidget *ui;

    QList<QString> defaultIdentifiers;
};

#endif // GENERATEGRAPHWIDGET_H

