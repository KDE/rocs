/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2011-2013 Andreas Cord-Landwehr <cola@uni-paderborn.de>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef GENERATEGRAPHWIDGET_H
#define GENERATEGRAPHWIDGET_H

#include "typenames.h"
#include "ui_generategraphwidget.h"
#include <QDialog>
#include <QWidget>

namespace GraphTheory
{

class GenerateGraphWidget : public QDialog
{
    Q_OBJECT

    enum GraphGenerator {
        MeshGraph,
        StarGraph,
        CircleGraph,
        RandomEdgeGraph,
        ErdosRenyiRandomGraph,
        RandomTree,
        RandomDag,
        PathGraph,
        CompleteGraph,
        CompleteBipartiteGraph
    };

public:
    explicit GenerateGraphWidget(GraphDocumentPtr document, QWidget *parent = nullptr);
    ~GenerateGraphWidget() override;

public Q_SLOTS:
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
     * Set the type of node elements for the generator.
     *
     * \param type is the NodeType ID
     */
    void setNodeType(int type);

    /**
     * Set the type of edges for the generator.
     *
     * \param type is the NodeType ID
     */
    void setEdgeType(int type);

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
    QPointF documentCenter() const;

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
     * Generate a random graph by assigning uniformly at random \p edges many edges to the set of nodes.
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
     * many node elements and then creating any possibly edge with probability \p edgeProbability.
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

    /**
     * Generate a random DAG by using a ranking algorithm.
     *
     * \param total_nodes is the number of nodes in the DAG
     * \param edgeProbability is the probability of creating an edge
     */
    void generateRandomDagGraph(int nodes, double edgeProbability);

    /**
     * Generate a path graph with specified number of nodes.
     *
     * \param pathSize is the number of nodes of the generated graph
     */
    void generatePathGraph(int pathSize);

    /**
     * Generate a complete graph with specified number of nodes.
     *
     * \param nodes is the number of nodes of the generated graph
     */
    void generateCompleteGraph(int nodes);

    /**
     * Generate a complete bipartite graph with specified number of nodes.
     *
     * \param nodes_left is the number of nodes in the left set of the bipartite graph.
     * \param nodes_right is the number of nodes in the right set of the bipartite graph.
     */
    void generateCompleteBipartiteGraph(int nodes_left, int nodes_right);

    GraphDocumentPtr m_document;
    int m_seed;
    NodeTypePtr m_nodeType;
    EdgeTypePtr m_edgeType;
    QString m_identifier;
    GraphGenerator m_graphGenerator;

    QHash<GraphGenerator, QString> m_defaultIdentifiers;

    Ui::GenerateGraphWidget *ui;
};
}

#endif
