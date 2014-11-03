/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TEST_KERNEL_H
#define TEST_KERNEL_H

#include <QObject>

namespace GraphTheory
{
class GraphDocument;
class Node;
class Edge;
}

using namespace GraphTheory;

class TestKernel : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    /** set up an engine and start execution **/
    void engineSetup();
    /** test script accessibility of graph document objects **/
    void graphObjectAccess();
    /** test access to graph objects of specific type (nodetype, edgetype) **/
    void graphObjectAccessWithTypes();
    /** test edge, inEdges, outEdges methods **/
    void edgeAccessMethods();
    /** test edge, inEdges, outEdges methods for edges of specific type **/
    void edgeAccessMethodsWithTypes();
    /** test node properties (id, color, x, y) **/
    void nodeProperties();
    /** test node dynamic properties **/
    void nodeDynamicProperties();
    /** test edge properties (from, to, directed) **/
    void edgeProperties();
    /** test edge dynamic properties **/
    void edgeDynamicProperties();
    /** test node type access **/
    void nodeTypes();
    /** test node type access **/
    void edgeTypes();
    /** test neighborship accessors: neighbors(), successors(), precedessors() **/
    void neighborships();
};

#endif
