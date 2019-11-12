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
};
}

#endif
