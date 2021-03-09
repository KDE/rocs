/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */


#include "testradiallayout.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "editor.h"
#include "modifiers/topology.h"
#include <QTest>
#include <QVector2D>
#include <math.h>

#include <iostream>

using namespace GraphTheory;

void RadialLayoutTest::testMargin()
{
    constexpr qreal EPS = 1.e-4;

    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    Node::create(document);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(status);

    for (const NodePtr node : document->nodes()) {
        QVERIFY(node->x() - EPS >= margin);
        QVERIFY(node->y() - EPS >= margin);
    }

}

void RadialLayoutTest::emptyGraph()
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(status);
}

void RadialLayoutTest::singleNode()
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    Node::create(document);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(status);
}

void RadialLayoutTest::cycleGraph()
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);
    NodePtr nodeC = Node::create(document);

    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);

    Edge::create(nodeA, nodeB)->setType(edgeType);
    Edge::create(nodeA, nodeC)->setType(edgeType);
    Edge::create(nodeC, nodeB)->setType(edgeType);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(not status);
}

void RadialLayoutTest::disconnectedGraph()
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    Node::create(document);
    Node::create(document);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(not status);
}

void RadialLayoutTest::unidirectionalEdge()
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);

    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Unidirectional);

    Edge::create(nodeA, nodeB)->setType(edgeType);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(not status);
}

void RadialLayoutTest::minimumNodeDistance()
{
    constexpr qreal EPS = 1.e-4;

    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    NodePtr nodeA = Node::create(document);
    NodePtr nodeB = Node::create(document);

    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);

    Edge::create(nodeA, nodeB)->setType(edgeType);

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = 50.;
    const NodePtr root = nullptr;
    const qreal wedgeAngle = M_PI * 2.;
    const qreal rotationAngle = 0.;
    const bool status = Topology::applyRadialLayoutToTree(document, nodeRadius,  margin,
                                                          nodeSeparation, root, wedgeAngle,
                                                          rotationAngle);
    QVERIFY(status);

    const QVector2D positionA(nodeA->x(), nodeA->y());
    const QVector2D positionB(nodeB->x(), nodeB->y());
    const qreal distance = (positionA - positionB).length();
    QVERIFY(distance + EPS >= 2. * nodeRadius + nodeSeparation);
}


QTEST_MAIN(RadialLayoutTest)
