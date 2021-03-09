/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    /** test access of nodes by node(id) **/
    void nodeAccessMethods();
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
    /** test neighborship accessors: neighbors(), successors(), precedessors() with specified types**/
    void neighborshipsWithTypes();
    /** test automatic creation of dynamic properties: nodes/edges can store arbitrary properties **/
    void automaticScriptObjectPropertyGeneration();
    /** test script API to create nodes **/
    void createNode();
    /** test script API to create edges **/
    void createEdge();
    /** test script API to delete nodes **/
    void deleteNode();
    /** test script API to delete edges **/
    void deleteEdge();
    /** test Node::distance function **/
    void distance();
};

#endif
