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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "topology.h"
#include "graphdocument.h"
#include "edge.h"
#include "logging_p.h"

#include <QList>
#include <QPair>
#include <QVector>
#include <QVector2D>
#include <QtMath>
#include <QPointF>
#include <QRandomGenerator>

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


typedef QPair<int, int> RemappedEdge;
struct RemappedGraph {
    int numberOfNodes;
    QMap<NodePtr, int> nodeToIndexMap;
    QVector<RemappedEdge> edges;
};

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
    std::sort(xList.begin(), xList.end());
    std::sort(yList.begin(), yList.end());

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
        std::sort(xList.begin(), xList.end());
        std::sort(yList.begin(), yList.end());

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

/** @brief Calculates the size of a square in which a number of circles fit well. 
 *
 * Given the number of circles and their radius, heuristically computes the length of the side
 * of a square in which all circles can easily be placed without intersecting each other.
 * Use this to figure out how big a square should be contain the drawing of graph well.
 *
 * By easily, understand the following:
 * Consider a square with side length computed by this method. An algorithm that places circles
 * at random positions inside this square, with uniform probability, is expected to generate
 * little or no intersection between circles.
 *
 */
qreal squareSideRandomPlacementHeuristic(const qreal radius, const int numberOfCircles)
{
    if (numberOfCircles < 2) {
        return 2 * radius;
    }

    //This formula was obtained by a mix of experimentation and calculations.
    //return qMax(4.26 * numberOfCircles - 3.5, 2.) * radius;
    
    return qSqrt(15.59 * numberOfCircles - 21.32) * radius;   
}

QMap<NodePtr, int> mapNodesToIndexes(const NodeList& nodes)
{
    int nextIndex = 0;
    QMap<NodePtr, int> nodeToIndexMap;
    foreach(const NodePtr node, nodes) {
        nodeToIndexMap[node] = nextIndex;
        nextIndex++;
    }
    return nodeToIndexMap;
}

QVector<RemappedEdge> getRemappedEdges(const EdgeList& edges,
                                       const QMap<NodePtr, int>& nodeToIndexMap)
{
    QVector<RemappedEdge> remappedEdges;
    foreach(const EdgePtr edge, edges) {
        const int from = nodeToIndexMap[edge->from()];
        const int to = nodeToIndexMap[edge->to()];
        remappedEdges.push_back(RemappedEdge(from, to));
    }
    return remappedEdges;
}

RemappedGraph remapGraph(const GraphDocumentPtr document) {
    RemappedGraph remappedGraph;
    remappedGraph.numberOfNodes = document->nodes().size();
    remappedGraph.nodeToIndexMap = mapNodesToIndexes(document->nodes());
    remappedGraph.edges = getRemappedEdges(document->edges(), remappedGraph.nodeToIndexMap);
    return remappedGraph;
}

void moveNodes(const NodeList& nodes, const QMap<NodePtr, int>& nodeToIndexMap,
               const QVector<QPointF>& positions)
{
    foreach(const NodePtr node, nodes) {
        const int index = nodeToIndexMap[node];
        const QPointF& position = positions[index];
        node->setX(position.x());
        node->setY(position.y());
    }
}

QVector<QPointF> getCurrentPositions(const NodeList& nodes,
                                    const QMap<NodePtr, int>& nodeToIndexMap)
{
    QVector<QPointF> positions(nodes.size());
    foreach(const NodePtr node, nodes) {
        const int index = nodeToIndexMap[node];
        positions[index] = QPointF(node->x(), node->y());
    }
    return positions;
}


QVector2D randomDirection(QRandomGenerator& randomGenerator) {
    const qreal angle = randomGenerator.bounded(2 * M_PI);
    return QVector2D(qCos(angle), qSin(angle));
}

QPointF projectIntoRectangle(const QPointF& point, const qreal minX, const qreal maxX,
                               const qreal& minY, const qreal& maxY)
{
    const qreal x = qMin(qMax(point.x(), minX), maxX);
    const qreal y = qMin(qMax(point.y(), minY), maxY);
    return QPointF(x, y);
}

/** 
 * Lays a graph out using an adaptation of the Fruchterman-Reingold algorithm. 
 *
 * @param graph The graph to be laid out.
 * @param areaFactor A positive constant that imprecisely indicates how spread the graph should be.
 * @param repellingForce A constant that controls how strong is the repelling force between nodes.
 * @param attractionForce A constant that controls how strong is the attraction force between nodes
 *                        that are neighbours.
 * @param minX is the minimum x-coordinate the center of a node can have.
 * @param maxX is the maximum x-coordinate the center of a node can have.
 * @param minY is the minimum y-coordinate the center of a node can have.
 * @param maxY is the maximum y-coordinate the center of a node can have.
 * @param nodeRadius is the radius of each node.
 * @param initialTemperature is the temperature to start the simulation.
 * @param numberOfIterations Number of iterations to be realized in the simulation.
 * @param initialPositions The initial positions of the nodes.
 * @param randomGenerator The random number generator engine to be used if necessary.
 *
 * @return The position of the nodes after the simulation.
 */
QVector<QPointF> forceBasedLayout(const RemappedGraph& graph, const qreal areaFactor,
                                  const qreal repellingForce, const qreal attractionForce,
                                  const qreal minX, const qreal maxX, const qreal minY,
                                  const qreal maxY, const qreal nodeRadius,
                                  const qreal initialTemperature, const int numberOfIterations,
                                  const QVector<QPointF>& initialPositions,
                                  QRandomGenerator& randomGenerator)
{
    Q_ASSERT(maxX > minX);
    Q_ASSERT(maxY > minY);
    Q_ASSERT(areaFactor > 0.);
    Q_ASSERT(graph.numberOfNodes > 0);

    //Constant used to calculate the forces acting on each node.
    const qreal area = (maxX - minX) * (maxY - minY);
    const qreal k = areaFactor * qSqrt(area / graph.numberOfNodes);

    //Length of the diagonal of the rectangle.
    const qreal diagonalLength = qSqrt(qPow(maxX - minX, 2) + qPow(maxY - minY, 2));

    //Maximum distance at which repelling forces do act.
    const qreal repellingForceRadius = qMax(3 * nodeRadius, diagonalLength / 2);
    
    QVector<QPointF> currentPositions = initialPositions;
    QVector<QVector2D> nodeForce(graph.numberOfNodes);
    for (int iteration = 0; iteration < numberOfIterations; iteration++) {
        //Clear forces from the previous iteration.
        nodeForce.fill(QVector2D());

        //Calculates the repelling forces.
        for (int i = 0; i < graph.numberOfNodes; i++) {
            for (int j = i + 1; j < graph.numberOfNodes; j++) {
                QVector2D direction(currentPositions[j] - currentPositions[i]);
                const qreal distance = direction.length();

                //Avoid using repelling forces between nodes that are too far from each other.
                //Even when small, this forces tend to make nodes go to the sides of the rectangle.
                if (distance > repellingForceRadius) {
                    continue;
                }
                
                //Adaptation of the original Fruchterman-Reingold calculation to consider the
                //radius of nodes, avoiding intersection between pairs of nodes.
                //Using k insted of k * k in the force calculation seems to lead to better results.
                const qreal correctedDistance = qMax(distance - 2 * nodeRadius, 
                                                     1. / graph.numberOfNodes);
                const qreal force = repellingForce * k * k / correctedDistance;

                //If the distance is too small, pick a random direction to avoid the case in
                //which two nodes have the same position.
                if (distance < nodeRadius) {
                    direction = randomDirection(randomGenerator);
                } else {
                    direction.normalize();
                }

                nodeForce[i] -= force * direction;
                nodeForce[j] += force * direction;
            }
        }

        //Calculates the attraction forces.
        for (const RemappedEdge& edge : graph.edges) {
            const int i = edge.first;
            const int j = edge.second;
            QVector2D direction(currentPositions[j] - currentPositions[i]);
            const qreal distance = direction.length();

            //Do not use attraction forces between nodes that are already too close.
            if (distance < 3 * nodeRadius) {
                continue;
            }

            direction.normalize();
            const qreal force = attractionForce * distance * distance / k;
            
            nodeForce[i] += force * direction;
            nodeForce[j] -= force * direction;
        }


        //Calculates the current temperature using a liner cooling schedule.
        const qreal temperature = initialTemperature * (numberOfIterations - iteration) /
                                  numberOfIterations;

        //Moves nodes, keeping their coordinates inside the allowed ranges.
        for (int i = 0; i < graph.numberOfNodes; i++) { 
            const qreal displacement = qMin<qreal>(nodeForce[i].length(), temperature);
            const QVector2D direction = nodeForce[i].normalized();
            const QPointF target(currentPositions[i].x() + displacement * direction.x(),
                                 currentPositions[i].y() + displacement * direction.y());
            currentPositions[i] = projectIntoRectangle(target, minX, maxX, minY, maxY);
        }

    }

    return currentPositions;
}

QVector<QPointF> randomLayout(const RemappedGraph& graph, const qreal minX, const qreal maxX,
                              const qreal minY, const qreal maxY, QRandomGenerator& randomGenerator)
{
    Q_ASSERT(maxX > minX);
    Q_ASSERT(maxY > minY);
    QVector<QPointF> positions(graph.numberOfNodes);
    for (int i = 0; i < graph.numberOfNodes; i++) {
        positions[i].setX(randomGenerator.bounded(maxX - minX) + minX);
        positions[i].setY(randomGenerator.bounded(maxY - minY) + minY);
    }
    return positions;
}

void translateGraphToUpperLeftCorner(const qreal minX, const qreal maxX, const qreal minY,
                                    const qreal maxY, QVector<QPointF>& positions)
{
    qreal xDisplacement = maxX - minX;
    qreal yDisplacement = maxY - minY;
    for (const QPointF& point : positions) {
        xDisplacement = qMin(xDisplacement, point.x() - minX);
        yDisplacement = qMin(yDisplacement, point.y() - minY);
    }

    for (QPointF& point : positions) {
        point.setX(point.x() - xDisplacement);
        point.setY(point.y() - yDisplacement);
    }
}

void Topology::applyForceBasedLayout(GraphDocumentPtr document, const qreal nodeRadius,
                                     const qreal margin, const qreal areaFactor,
                                     const qreal repellingForce, const qreal attractionForce,
                                     const bool randomizeInitialPositions, const quint32 seed)
{
    //There is nothing to do with an empty graph.
    if (document->nodes().empty()) {
        return;
    }

    QRandomGenerator randomGenerator(seed);

    //Gets a new representation of the graph for efficiency purposes
    RemappedGraph graph = remapGraph(document);

    //Computes the square in which the center of the nodes should be placed.
    //This is done heuristically so that there is enough room to move nodes around easily.
    //Because the heuristic used considers only circles, one extra circle is created for each edge.
    //The reasoning is that graphs with more edges need more space to drawn nicely.
    const int numberOfCircles = graph.numberOfNodes + qMin(graph.edges.size(),
                                                           5 * graph.numberOfNodes);
    const qreal circleRadius = 2 * nodeRadius;
    const qreal side = squareSideRandomPlacementHeuristic(circleRadius, numberOfCircles);
    const qreal minX = margin + nodeRadius;
    const qreal maxX = minX + side;
    const qreal minY = margin + nodeRadius;
    const qreal maxY = minY + side;

    //Computes the initial positions.
    QVector<QPointF> initialPositions;
    if (randomizeInitialPositions) {
        initialPositions = randomLayout(graph, minX, maxX, minY,maxY, randomGenerator);
    } else {
        initialPositions = getCurrentPositions(document->nodes(), graph.nodeToIndexMap);
    }

    
    //In order to converge properly, it makes sense that the number of iterations increases as
    //the number of nodes increases. For very small graphs, a minimum number of iterations
    //should be realized so the algorithm has the chance to find a nice layout.
    const int numberOfIterations = qMax(graph.numberOfNodes, 100);

    //The temperature indicates how far a node can be moved in a single iteration.
    //Initially, this value should be big enough to enable every node to go anywhere.
    //Here, this is set so that after freeIterations each node can be anywhere.
    const int freeIterations = numberOfIterations / 10 + 1;
    const qreal initialTemperature = 2. * qSqrt(2.) * side * numberOfIterations / freeIterations /
                                     (2 * numberOfIterations - freeIterations + 1);

    //Computes the layout.
    QVector<QPointF> positions = forceBasedLayout(graph, areaFactor, repellingForce,
                                                  attractionForce, minX, maxX, minY, maxY,
                                                  nodeRadius, initialTemperature,
                                                  numberOfIterations, initialPositions,
                                                  randomGenerator);

    
    //The generated layout may have some unused space above the graph and to the left of the graph.
    translateGraphToUpperLeftCorner(minX, maxX, minY, maxY, positions); 


    //Moves nodes to their final positions.
    moveNodes(document->nodes(), graph.nodeToIndexMap, positions);
}
