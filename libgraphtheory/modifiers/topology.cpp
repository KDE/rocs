/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "topology.h"
#include "graphdocument.h"
#include "edge.h"
#include "logging_p.h"

#include <algorithm>

#include <QList>
#include <QPair>
#include <QVector>
#include <QVector2D>
#include <QStack>
#include <QQueue>
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
    QVector<QVector<int>> adjacency;
};

// handle boost exceptions
namespace boost {
    void throw_exception(std::exception const &e) {
        qCCritical(GRAPHTHEORY_GENERAL) << "Exception:" << e.what();
    }
}

void Topology::applyMinCutTreeAlignment(const NodeList &nodes)
{
    // nodes must be at least of length 2, and two nodes cannot have crossing edges
    if (nodes.count() < 3) {
        return;
    }

    PositionVec position_vec(nodes.count());

    // set box inside which we may reposition
    QList<qreal> xList;
    QList<qreal> yList;
    for (const NodePtr& node : nodes) {
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
    for (const NodePtr& node :nodes) {
        node_mapping[node] = counter++;
    }

    const auto documentEdges = nodes.first()->document()->edges();
    QVector<BoostEdge> edges(documentEdges.size());

    counter = 0;
    for (const EdgePtr& edge : documentEdges) {
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
    for (const NodePtr &node : nodes) {
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
    for (const NodePtr& node : nodes) {
        Vertex v = boost::vertex(node_mapping[node], graph);
        node->setX(positionMap[v][0]);
        node->setY(positionMap[v][1]);
    }
}

void Topology::applyCircleAlignment(const NodeList &nodes, qreal radius)
{
    if (nodes.length() == 0) {
        return;
    }

    PositionVec position_vec(nodes.count());

    if(radius == 0) {
        // set box inside which we may reposition
        QList<qreal> xList;
        QList<qreal> yList;
        for (const NodePtr& node : nodes) {
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
    for (const NodePtr &node : nodes) {
        node_mapping[node] = counter++;
    }

    const auto documentEdges = nodes.first()->document()->edges();
    QVector<BoostEdge> edges(documentEdges.size());

    counter = 0;
    for (const EdgePtr& edge : documentEdges) {
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
    for (const NodePtr &node : nodes) {
        positionMap[counter][0] = node->x();
        positionMap[counter][1] = node->y();
        counter++;
    }

    // layout to circle
    boost::circle_graph_layout<Graph, PositionMap>( graph,
                                                    positionMap,
                                                    radius);

    // put nodes at whiteboard as generated
    for (const NodePtr &node : nodes) {
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

/* Maps the nodes in a node list to indexes, starting from the index 0.
 */
QMap<NodePtr, int> mapNodesToIndexes(const NodeList& nodes)
{
    int nextIndex = 0;
    QMap<NodePtr, int> nodeToIndexMap;
    for (const NodePtr& node : nodes) {
        nodeToIndexMap[node] = nextIndex;
        nextIndex++;
    }
    return nodeToIndexMap;
}

QVector<RemappedEdge> getRemappedEdges(const EdgeList& edges,
                                       const QMap<NodePtr, int>& nodeToIndexMap)
{
    QVector<RemappedEdge> remappedEdges;
    for (const EdgePtr& edge : edges) {
        const int from = nodeToIndexMap[edge->from()];
        const int to = nodeToIndexMap[edge->to()];
        remappedEdges.push_back(RemappedEdge(from, to));
    }
    return remappedEdges;
}

/* 
 * Extracts the graph from a GraphDocument to a representation that is more convenient for
 * the graph layout algorithms. 
 */
RemappedGraph remapGraph(const GraphDocumentPtr document) {
    RemappedGraph remappedGraph;
    remappedGraph.numberOfNodes = document->nodes().size();
    remappedGraph.nodeToIndexMap = mapNodesToIndexes(document->nodes());
    remappedGraph.edges = getRemappedEdges(document->edges(), remappedGraph.nodeToIndexMap);
    remappedGraph.adjacency.resize(remappedGraph.numberOfNodes);
    for (const QPair<int, int>& edge : remappedGraph.edges) {
        remappedGraph.adjacency[edge.first].push_back(edge.second);
        remappedGraph.adjacency[edge.second].push_back(edge.first);
    }
    return remappedGraph;
}

/* Updates the positions of the nodes in a NodeList.
 *
 * For performance reasons, this should not be done all the time.
 * Every time a coordinate of a node changes, a Qt signal is emitted.
 */
void moveNodes(const NodeList& nodes, const QMap<NodePtr, int>& nodeToIndexMap,
               const QVector<QPointF>& positions)
{
    for (const NodePtr& node : nodes) {
        const int index = nodeToIndexMap[node];
        const QPointF& position = positions[index];
        node->setX(position.x());
        node->setY(position.y());
    }
}

/* Extracts the current positions of the nodes in a NodeList.
 */
QVector<QPointF> getCurrentPositions(const NodeList& nodes,
                                    const QMap<NodePtr, int>& nodeToIndexMap)
{
    QVector<QPointF> positions(nodes.size());
    for (const NodePtr& node : nodes) {
        const int index = nodeToIndexMap[node];
        positions[index] = QPointF(node->x(), node->y());
    }
    return positions;
}


/* Computes a unit vector with direction chosen at random.
 * All the directions have the same probability of being chosen.
 */
QVector2D randomDirection(QRandomGenerator& randomGenerator) {
    const qreal angle = randomGenerator.bounded(2 * M_PI);
    return QVector2D(qCos(angle), qSin(angle));
}

/* Given point, computes the closest point to it that lies inside the specified rectangle.
 */
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

/* Randomly assigns positions to the nodes of a graph. The positions of the nodes are
 * chosen independently from each other with uniform probability inside the specified
 * rectangle.
 */
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

/* Translates de nodes of a graph so that the graph touches the left and the upper sides of the
 * specified rectangle.
 */
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

/* Returns the children of @p node in a Depth-First-Search tree.
 *
 * @param graph The graph being searched.
 * @param visited Indicates which nodes have already been visited by the Depth-First-Search.
 * @param node The current node of the search.
 */
QVector<int> getChildren(const RemappedGraph& graph, const QVector<bool>& visited, const int node)
{
    QVector<int> children;
    for (const int neighbour : graph.adjacency[node]) {
        if (not visited[neighbour]) {
            children.push_back(neighbour);
        }
    }
    return children;
}

/* Reorders the children heuristically to avoid the need of very long edges.
 *
 * The length of the edges between a node and its children depends on the angle formed by the
 * centers of the two children and the origin. If this angle is very small, long edges are needed
 * to avoid the intersection of two nodes. The smallest of these angles is determined by the
 * number of leafs in the sub-trees rooted at two consecutive nodes. This function mixes
 * nodes with small and large number of leafs in their sub-trees in order to avoid having two
 * consecutive children with a small number of leafs in their sub-trees.
 */
void reorderChildrenForRadialLayout(const QVector<int>& numberOfLeafs, QVector<int>& children)
{
    const auto numberOfLeafsComparator = [&numberOfLeafs](const int i, const int j) {
            return numberOfLeafs[i] < numberOfLeafs[j];
        };

    std::sort(children.begin(), children.end(), numberOfLeafsComparator);

    const  int numberOfChildren = children.size();
    int small = 0;
    int large = numberOfChildren - 1;
    while (small < large) {
        std::swap(children[small], children[large]);
        small += 2;
        large -= 2;
    }
}

/* Calculates a wedge angle that guarantees that no edge crosses can happen.
 *
 */
qreal constrainWedgeAngle(const QVector<int>& numberOfLeafs, const qreal wedgeAngle,
                          const qreal circleRadius, const int node,
                          const QVector<int>& children, const qreal circleRadiusForChildren)
{
    if (children.size() <= 1 or circleRadius == 0.) {
        return wedgeAngle;
    }

    //Proportion of the are in the sides of the wedge in which there will be no edges.
    const qreal leftProportion = qreal(numberOfLeafs[children[0]]) / numberOfLeafs[node] / 2.;
    const qreal rightProportion = qreal(numberOfLeafs[children.back()]) / numberOfLeafs[node] / 2.;
    const qreal maximumProportion = qMax(leftProportion, rightProportion);

    //Limit the wedge angle to guarantee no crosses between edges.
    const qreal maximumWedgeAngle = 2. * acos(circleRadius / circleRadiusForChildren) /
                                    maximumProportion;

    return qMin(wedgeAngle, maximumWedgeAngle);
}

/* Checks whether a given circle radius is valid for placing the children of a node.
 *
 * In order to be valid, the radius must be large enough to allow the radialLayoutHelper
 * to place the node in such a way that guarantees that no edge crosses or node intersections
 * can happen.
 */
bool isValidCircleRadiusForChildren(const QVector<int>& numberOfLeafs, const qreal nodeRadius,
                                    const qreal wedgeAngle, const qreal circleRadius,
                                    const qreal nodeSeparation, const int node,
                                    const QVector<int>& children,
                                    const qreal circleRadiusForChildren)
{
    const qreal constrainedWedgeAngle = constrainWedgeAngle(numberOfLeafs, wedgeAngle, circleRadius,
                                                            node, children,
                                                            circleRadiusForChildren);
    const qreal minimumDistance = 2. * nodeRadius + nodeSeparation;
    const qreal squaredMinimumDistance = minimumDistance * minimumDistance;

    const qreal nodeAngle = constrainedWedgeAngle / 2.;
    QPointF nodePosition(circleRadius * qCos(nodeAngle), circleRadius * qSin(nodeAngle));

    const int numberOfChildren = children.size();
    qreal childRotation = 0.;
    QPointF previousChildPosition;
    for (int i = 0; i < numberOfChildren; i++) {
        const int child = children[i];
        const qreal childWedgeAngle = constrainedWedgeAngle * numberOfLeafs[child] /
                                      numberOfLeafs[node];
        const qreal childAngle = childRotation + childWedgeAngle / 2.;
        childRotation += childWedgeAngle;
        QPointF childPosition(circleRadiusForChildren * qCos(childAngle),
                              circleRadiusForChildren * qSin(childAngle));

        //Checks the distance between a node and of its children.
        const QPointF nodeChildDifference = childPosition - nodePosition;
        const qreal squaredNodeChildDistance(QPointF::dotProduct(nodeChildDifference,
                                                                 nodeChildDifference));
        if (squaredNodeChildDistance < squaredMinimumDistance) {
            return false;
        }

        //Checks the distance between two children.
        if (i > 0) {
            const QPointF difference = childPosition - previousChildPosition;
            const qreal squaredDistanceBetweenChildren = QPointF::dotProduct(difference,
                                                                             difference);
            if (squaredDistanceBetweenChildren < squaredMinimumDistance) {
                return false;
            }
        }
        previousChildPosition = childPosition;
    }


    return true;
}

/* Calculates the radius of the circle at which the center of the children will be placed.
 * outside of.
 *
 * This functions the minimum radius that guarantees that no edge crosses or circle intersections
 * are possible, considering that a node and its children are placed by the radialLayoutHelper
 * function.
 */
qreal calculateCircleRadiusForChildren(const QVector<int>& numberOfLeafs, const qreal nodeRadius,
                                       const qreal wedgeAngle, const qreal circleRadius,
                                       const qreal nodeSeparation, const int node,
                                       const QVector<int>& children)
{
    //Binary search is used to find the smallest valid radius up to a tolerance.
    //In order to avoid infinite loop due to rounding errors, a maximum number of iterations
    //is specified.
    constexpr int MAXIMUM_NUMBER_OF_ITERATIONS = 100;
    constexpr qreal TOLERANCE = 1.e-4;
   
    //Finds a suitable lower bound for the binary search
    const qreal minimumDistanceBetweenCenters = 2. * nodeRadius + nodeSeparation;
    qreal deltaRadiusLowerBound = qSqrt(qPow(minimumDistanceBetweenCenters, 2.) + 
                                        qPow(circleRadius, 2.)) - circleRadius;
    
    //Finds a suitable upper bound for the binary search.
    qreal deltaRadiusUpperBound = deltaRadiusLowerBound;
    for (int iteration = 0; iteration < MAXIMUM_NUMBER_OF_ITERATIONS; iteration++) {
        const qreal circleRadiusForChildren = circleRadius + deltaRadiusUpperBound;
        if (isValidCircleRadiusForChildren(numberOfLeafs, nodeRadius, wedgeAngle, circleRadius,
                                           nodeSeparation, node, children,
                                           circleRadiusForChildren)) {
            break;
        }

        deltaRadiusUpperBound *= 2.;
    }

    //Searches for the minimum valid radius
    for (int iteration = 0; iteration < MAXIMUM_NUMBER_OF_ITERATIONS; iteration++) {
        if (deltaRadiusUpperBound - deltaRadiusLowerBound < TOLERANCE) {
            break;
        }

        const qreal deltaRadius = (deltaRadiusUpperBound + deltaRadiusLowerBound) / 2.;
        const qreal circleRadiusForChildren = circleRadius + deltaRadius;
        if (isValidCircleRadiusForChildren(numberOfLeafs, nodeRadius, wedgeAngle, circleRadius,
                                           nodeSeparation, node, children,
                                           circleRadiusForChildren)) {
            deltaRadiusUpperBound = deltaRadius;
        } else {
            deltaRadiusLowerBound = deltaRadius;
        }
    }

    return circleRadius + deltaRadiusUpperBound;
}

/* Helper function that calculates the radial layout recursively.
 *
 * This function calculates positions for each node in the sub-tree rooted at @p node.
 * Consider the circle C centered at the origin with radius @circleRadius.
 * Consider the wedge W, formed by the points with polar angle between @p rotationAngle and
 * @p rotationAngle + @p wedgeAngle.
 * The center of each node in the sub-tree rooted at @p node is placed outside or in the border of
 * C and inside W. The parameters that define C and W are chosen in such a way that edge crosses
 * are impossible and the distance between any pair of nodes is at least @p nodeSeparation.
 *
 * @param graph A tree graph.
 * @param numberOfLeafs The number of leafs in the sub-tree rooted at each node.
 * @param nodeRadius The radius of the circles used to draw nodes.
 * @param wedgeAngle The internal angle of the wedge in which nodes are placed.
 * @param rotationAngle The angle between the x-axis and the wedge in which nodes are placed.
 * @param circleRadius The radius of the circles whose exterior is used to place nodes.
 * @param nodeSeparation Lower bound on the distance between nodes
 * @param node Root of the current sub-tree.
 * @param visited Flags used to indicate which nodes have already been placed.
 *                Initially, all positions of @p visited must be false.
 *                This function changes @p visited.
 * @param positions A place to write the position of each node.
 */
void radialLayoutHelper(const RemappedGraph& graph, const QVector<int>& numberOfLeafs,
                        const qreal nodeRadius, const qreal wedgeAngle, const qreal rotationAngle,
                        const qreal circleRadius, const qreal nodeSeparation, const int node,
                        QVector<bool>& visited, QVector<QPointF>& positions)
{
    visited[node] = true;
    
    //Places current node at the center of the wedge.
    const qreal nodeAngle = wedgeAngle / 2. + rotationAngle;
    positions[node].setX(circleRadius * qCos(nodeAngle));
    positions[node].setY(circleRadius * qSin(nodeAngle));

    QVector<int> children = getChildren(graph, visited, node);
    reorderChildrenForRadialLayout(numberOfLeafs, children);

    const qreal circleRadiusForChildren = calculateCircleRadiusForChildren(numberOfLeafs,
            nodeRadius, wedgeAngle, circleRadius, nodeSeparation, node, children);


    const qreal constrainedWedgeAngle = constrainWedgeAngle(numberOfLeafs, wedgeAngle, circleRadius,
                                                            node, children,
                                                            circleRadiusForChildren);

    qreal childRotationAngle = rotationAngle + (wedgeAngle - constrainedWedgeAngle) / 2.;
    for (const int child : children) { 
        const qreal childWedgeAngle = constrainedWedgeAngle * numberOfLeafs[child] /
                                      numberOfLeafs[node];

        radialLayoutHelper(graph, numberOfLeafs, nodeRadius, childWedgeAngle, childRotationAngle,
                           circleRadiusForChildren, nodeSeparation, child, visited, positions);
        
        childRotationAngle += childWedgeAngle;
    }
}

/* Calculates the number of leafs in each sub-tree of a Depth-First-Search tree of @p graph.
 *
 * @param graph The graph to be searched.
 * @param node The current node.
 * @param visited Indicates which nodes have already been visited in the search.
 * @param numberOfLeafs A place to store the answer.
 */
void calculateNumberOfLeafs(const RemappedGraph& graph, const int node, QVector<bool>& visited,
                            QVector<int>& numberOfLeafs)
{
    visited[node] = true;

    numberOfLeafs[node] = 0;
    bool isLeaf = true;
    for (const int neighbour : graph.adjacency[node]) {
        if (not visited[neighbour]) {
            isLeaf = false;
            calculateNumberOfLeafs(graph, neighbour, visited, numberOfLeafs);
            numberOfLeafs[node] += numberOfLeafs[neighbour];
        }
    }

    if (isLeaf) {
        numberOfLeafs[node] = 1;
    }
}

/* Finds a center of a tree.
 *
 * A center is a node with minimum eccentricity (i.e. distance to most distance node).
 * 
 * @param graph A tree graph.
 */
int findTreeCenter(const RemappedGraph& graph) {
    int center = 0;
    QQueue<int> queue;
    QVector<int> degree(graph.numberOfNodes);
    QVector<bool> visited(graph.numberOfNodes);
    for (int node = 0; node < graph.numberOfNodes; node++) {
        degree[node] = graph.adjacency[node].size();
        if (degree[node] == 1) {
            queue.enqueue(node);
        }
    }

    while (not queue.isEmpty()) {
        const int node = queue.dequeue();
        visited[node] = true;
        center = node;
        for (const int neighbour : graph.adjacency[node]) {
            if (not visited[neighbour]) {
                degree[neighbour]--;
                if (degree[neighbour] == 1) {
                    queue.enqueue(neighbour);
                }
            }
        }
    }

    return center;
}

/* Generates a radial layout for a tree.
 *
 * @param graph A tree graph.
 * @param minX Minimum x-coordinate that can be assigned to a node.
 * @param minY Minimum y-coordinate that can be assigned to a node.
 * @param nodeRadius The radius of the circles used to draw nodes.
 * @param nodeSeparation A lower bound on the distance between two nodes.
 * @param root Root node of the tree.
 * @param initialWedgeAngle Angle of the wedge used for the root.
 * @param initialRotationAngle Rotation angle for the root.
 */
QVector<QPointF> radialLayout(const RemappedGraph& graph, const qreal minX, const qreal minY,
                              const qreal nodeRadius, const qreal nodeSeparation, const int root,
                              const qreal initialWedgeAngle, const qreal initialRotationAngle)
{
    QVector<bool> visited(graph.numberOfNodes);
    QVector<int> numberOfLeafs(graph.numberOfNodes);
    calculateNumberOfLeafs(graph, root, visited, numberOfLeafs);

    visited.fill(false);
    QVector<QPointF> positions(graph.numberOfNodes);
    radialLayoutHelper(graph, numberOfLeafs, nodeRadius, initialWedgeAngle, initialRotationAngle,
                       0., nodeSeparation, root, visited, positions);


    translateGraphToUpperLeftCorner(minX, qInf(), minY, qInf(), positions); 
    
    return positions;
}

/* Checks whether all the edges of a graph are bidirectional.
 */
bool hasOnlyBidirectionalEdges(GraphDocumentPtr document)
{
    const auto edges = document->edges();
    for (const EdgePtr &edge : edges) {
        if (edge->type()->direction() != EdgeType::Bidirectional) {
            return false;
        }
    }
    return true;
}

/* Checks whether a graph is connected
*/
bool isConnected(const RemappedGraph& graph)
{
    if (graph.numberOfNodes == 0) {
        return true;
    }

    QVector<bool> visited(graph.numberOfNodes);
    QStack<int> stack;
    stack.push(0);
    visited[0] = true;
    while (not stack.empty()) {
        const int node = stack.pop();
        for (const int neighbour : graph.adjacency[node]) {
            if (not visited[neighbour]) {
                stack.push(neighbour);
                visited[neighbour] = true;
            }
        }
    }

    for (int i = 0; i < graph.numberOfNodes; i++) {
        if (not visited[i]) {
            return false;
        }
    }
    
    return true;
}

/* Checks whether a graph is a tree.
*/
bool isTree(const RemappedGraph& graph)
{
    if (graph.numberOfNodes - 1 != graph.edges.size()) {
        return false;
    }

    return isConnected(graph);
}

bool Topology::applyRadialLayoutToTree(GraphDocumentPtr document, const qreal nodeRadius,
                                       const qreal margin, const qreal nodeSeparation,
                                       const NodePtr root, const qreal wedgeAngle,
                                       const qreal rotationAngle)
{
    //There is nothing to do with an empty graph.
    if (document->nodes().empty()) {
        return true;
    }

    //Allow only bidirectional edges.
    if (not hasOnlyBidirectionalEdges(document)) {
        return false;
    }

    const RemappedGraph graph = remapGraph(document);
    
    if (not isTree(graph)) {
        return false;
    }

    int rootIndex = 0;
    if (root == nullptr) {
        rootIndex = findTreeCenter(graph);
    } else {
        rootIndex = graph.nodeToIndexMap[root];
    }

    const qreal minX = nodeRadius + margin;
    const qreal minY = nodeRadius + margin;
    
    QVector<QPointF> positions = radialLayout(graph, minX, minY, nodeRadius, nodeSeparation,
                                              rootIndex, wedgeAngle, rotationAngle);
   
    moveNodes(document->nodes(), graph.nodeToIndexMap, positions);

    return true;
}
