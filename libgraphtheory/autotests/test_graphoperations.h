/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TEST_GRAPHOPERATIONS_H
#define TEST_GRAPHOPERATIONS_H

#include <QObject>

namespace GraphTheory
{
class GraphDocument;
class Node;
class Edge;
}

using namespace GraphTheory;

class TestGraphOperations : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testDocumentCreateDelete();
    void testNodeCreateDelete();
    void testEdgeCreateDelete();
    void testNodeTypeCreateDelete();
    void testEdgeTypeCreateDelete();
    void testNodeDynamicProperties();
    void testEdgeDynamicProperties();
    void testNodeIdentifiers();
    void testUnidirectionalEdges();
    void testBidirectionalEdges();
    void testEdgesOfDifferentType();
    void testEdgeDirectionChange();
    void testDynamicPropertyRename();
};

#endif
