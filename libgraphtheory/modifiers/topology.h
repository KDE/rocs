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

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "typenames.h"
#include "graphtheory_export.h"

#define BOOST_MATH_DISABLE_FLOAT128 1

namespace GraphTheory
{

/** \brief this class provides topology modifiers for graphs
 *
 * Methods of this class either can be applied to graphs to
 * make unique changes or connected to specific re-format signals to
 * apply a given topology after every change of the structure.
 */
class GRAPHTHEORY_EXPORT Topology
{
public:
    Topology() = delete;

    /** \brief applies Fruchterman-Reingold cut minimization
     *
     * For the given node set this algorithm applies the Boost implementation
     * of the Frutherman-Reingold force directed layout algorithm to minimize
     * crossing edges. Data must be element of the same graph. The
     * crossings of all present edges contained in this graph are
     * minimized. This method directly modifies the node.
     * \param nodeList is the list of all nodes
     * \return void
     */
    static void applyMinCutTreeAlignment(NodeList nodes);

    /** \brief applies Circle topology to node set
     *
     * For the given node set this algorithm applies the Boost implementation
     * to create a circle layout. If a radius is specified, the circle will get the specified radius,
     * otherwise with no radius set the diameter of the minimal bounding box of all node positions
     * is used to determine the radius.
     * \param nodeList is the list of all nodes
     * \param radius to optionally specify target radius
     * \return void
     */
    static void applyCircleAlignment(NodeList nodes, qreal radius=0);

    /** \brief applies a default topology for undirected graphs
     *
     * Use this method to apply a best-fit topology to an undirected graph (though the
     * graph need not to be of type "Graph") only based on the node connections.
     * I.e., no possible present coordinates are respected.
     */
    static void directedGraphDefaultTopology(GraphDocumentPtr document);

    /** \brief applies a default topology for undirected graphs
     *
     * Use this method to apply a best-fit topology to an undirected graph (though the
     * graph need not to be of type "Graph") only based on the node connections.
     * I.e., no possible present coordinates are respected.
     */
    static void undirectedGraphDefaultTopology(GraphDocumentPtr document);


    /**
     * Applies a force based graph layout algorithm to the graph.
     *
     * If @p randomizeInitialPositions is @c true, the current node positions are ignored.
     * Otherwise, they are used as the initial layout.
     *
     * Using a value of 1 for the parameters @p areaFactor, @p repellingForce and @p attractionForce
     * should give a reasonable results.
     *
     * The control given by the parameter @p areaFactor is not precise, meaning that doubling its
     * value does not necessarily mean that the final layout will use an area twice as big.
     * The greater @p areaFactor is, the more the nodes tend to spread.
     *
     * @param document The graph document to be laid out.
     * @param nodeRadius The radius of the circles that are used to represent nodes.
     * @param margin The size of the top and left margins.
     * @param areaFactor A constant that imprecisely controls how much area the layout will take.
     * @param repellingForce The magnitude of the repelling force between nodes.
     * @param attractionForce The magnitude of the attraction force between neighbouring nodes.
     * @param randomizeInitialPositions Indicates whether the algorithm should start from a
     *                                  random layout.
     * @param seed Seed used for generating random numbers.
     */
    static void applyForceBasedLayout(GraphDocumentPtr document, const qreal nodeRadius,
                                      const qreal margin, const qreal areaFactor,
                                      const qreal repellingForce, const qreal attractionForce,
                                      const bool randomizeInitialPositions, const quint32 seed);


    /**
     * Applies a radial layout to a tree.
     *
     * @param document The graph document to be laid out. This document should represent a tree.
     * @param nodeRadius The radius of the circles that are used to represent nodes.
     * @param margin The size of the top and left margins.
     * @param nodeSeparation The minimum distance between two nodes.
     * @param root Node to be used as root. Use nullptr to indicate that the root should be selected
     *             automatically.
     * @param wedgeAngle Angle of the wedge into which the nodes should be placed.
     * @param rotation Angle to rotate all nodes around the root.
     *
     * @return true if the layout can be applied and false otherwise. The layout can be applied if
     *         @p document represents a tree.
     */
    static bool applyRadialLayoutToTree(GraphDocumentPtr document, const qreal nodeRadius,
                                        const qreal margin, const qreal nodeSeparation,
                                        const NodePtr root, const qreal wedgeAngle,
                                        const qreal rotationAngle);
};
}

#endif
