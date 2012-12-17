/*
    This file is part of Rocs.
    Copyright (C) 2011  Andreas Cord-Landwehr <phoenixx@uni-paderborn.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <QtCore/QVector>
#include <QtCore/QPair>

#include "RocsCoreExport.h"
#include "CoreTypes.h"

/** \brief this class provides topology modifiers for data structures
 *
 * Methods of this class either can be applied to data structures to
 * make unique changes or connected to specific re-format signals to
 * apply a given topology after every change of the structure.
 */
class ROCSLIB_EXPORT Topology
{
public:
    Topology();
    virtual ~Topology();

    /** \brief applies Fruchterman-Reingold cut minimization
     *
     * For the given data set this algorithm applies the Boost implementation
     * of the Frutherman-Reingold force directed layout algorithm to minimize
     * crossing edges. Data must be element of the same data structure. The
     * crossings of all present edges contained in this data structure are
     * minimized. This method directly modifies the data.
     * \param dataList is the list of data
     * \return void
     */
    void applyMinCutTreeAlignment(DataList dataList);

    /** \brief applies Circle topology to data set
     *
     * For the given data set this algorithm applies the Boost implementation
     * to create a circle layout. If a radius is specified, the circle will get the specified radius,
     * otherwise with no radius set the diameter of the minimal bounding box of all data element positions
     * is used to determine the radius.
     * \param dataList is the list of data
     * \param radius to optionally specify target radius
     * \return void
     */
    void applyCircleAlignment(DataList dataList, qreal radius=0);

    /** \brief applies a default topology for undirected graphs
     *
     * Use this method to apply a best-fit topology to an undirected graph (though the
     * data structure need not to be of type "Graph") only based on the node connections.
     * I.e., no possible present coordinates are respected.
     */
    void directedGraphDefaultTopology(DataStructurePtr dataStructure);

    /** \brief applies a default topology for undirected graphs
     *
     * Use this method to apply a best-fit topology to an undirected graph (though the
     * data structure need not to be of type "Graph") only based on the node connections.
     * I.e., no possible present coordinates are respected.
     */
    void undirectedGraphDefaultTopology(DataStructurePtr dataStructure);
};

#endif // TOPOLOGY_H
