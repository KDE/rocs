/*
 *  Copyright 2020  Dilson Almeida Guimarães <dilsonguim@gmail.com>
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


#include <QVector>
#include <QRandomGenerator>
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include "edgetype.h"
#include "editor.h"
#include "modifiers/topology.h"
#include "layoutevaluator.h"
#include <iostream>

using namespace GraphTheory;

constexpr int REPETITIONS = 30;

//Applies the force based layout algorithm with fixed parameters
void applyForceBasedLayout(GraphDocumentPtr document, const quint32 seed)
{
    constexpr qreal nodeRadius = 10.;
    constexpr qreal margin = 5.;
    constexpr qreal areaFactor = 1.;
    constexpr qreal repellingForce = 1.;
    constexpr qreal attractionForce = 1.;
    const bool randomizeInitialPositions = true;
    Topology::applyForceBasedLayout(document, nodeRadius, margin, areaFactor, repellingForce,
                                    attractionForce, randomizeInitialPositions, seed);
}

//Applies the radial tree layout algorithm with fixed parameters.
void applyRadialLayout(GraphDocumentPtr document, const quint32 seed)
{
    constexpr qreal nodeRadius = 10.;
    constexpr qreal margin = 5.;
    constexpr qreal nodeSeparation = 50.;
    constexpr qreal wedgeAngle = 2. * M_PI;
    constexpr qreal rotationAngle = 0.;
    const NodePtr root = nullptr;
    const bool status =  Topology::applyRadialLayoutToTree(document, nodeRadius, margin,
                                                           nodeSeparation, root, wedgeAngle,
                                                           rotationAngle);
    Q_ASSERT(status);
}

GraphDocumentPtr generatePath(const int numberOfNodes)
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);

    NodePtr previous = Node::create(document);
    for (int i = 1; i < numberOfNodes; i++) {
        NodePtr node = Node::create(document);
        EdgePtr edge = Edge::create(previous, node);
        edge->setType(edgeType);
        previous = node;
    }

    return document;
}

template<typename F>
void benchmarkOnPaths(const quint32 runSeed, F applyLayout, const char* name)
{
    QRandomGenerator runSeedGenerator(runSeed);
    for (int numberOfNodes = 10; numberOfNodes <= 50; numberOfNodes += 10) {
        GraphDocumentPtr document = generatePath(numberOfNodes);
        
        LayoutEvaluator evaluator;
        for (int repetition = 0; repetition < REPETITIONS; repetition++) { 
            applyLayout(document, runSeedGenerator.generate());
            evaluator.evaluateLayout(document);
        }

        std::cout << "[" << name << " on paths with " << numberOfNodes << " nodes.] {" << std::endl;
        evaluator.showResults(std::cout);
        std::cout << "} " << std::endl << std::endl;
    }
}

GraphDocumentPtr generateCompleteGraph(const int numberOfNodes)
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);
    
    for (int i = 0; i < numberOfNodes; i++) {
        const NodePtr nodeA = Node::create(document);
        for (const NodePtr nodeB : document->nodes()) {
            if (nodeA != nodeB) {
                EdgePtr edge = Edge::create(nodeA, nodeB);
                edge->setType(edgeType);
            }
        }
    }

    return document;
}


void benchmarkOnCompleteGraphs(const quint32 runSeed)
{
    QRandomGenerator runSeedGenerator(runSeed);
    for (int numberOfNodes = 5; numberOfNodes <= 20; numberOfNodes += 5) {
        GraphDocumentPtr document = generateCompleteGraph(numberOfNodes);

        LayoutEvaluator evaluator;
        for (int repetition = 0; repetition < REPETITIONS; repetition++) {
            applyForceBasedLayout(document, runSeedGenerator.generate());
            evaluator.evaluateLayout(document);
        }

        std::cout << "[Force based layout on complete graphs with " << numberOfNodes << 
                  " ndoes.] {" << std::endl;
        evaluator.showResults(std::cout);
        std::cout << "} " << std::endl << std::endl;
    }
}

GraphDocumentPtr generateCircle(const int numberOfNodes)
{
    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);

    NodePtr first = Node::create(document);
    NodePtr previous = first;
    for (int i = 1; i < numberOfNodes; i++) {
        NodePtr node = Node::create(document);
        Edge::create(previous, node)->setType(edgeType);
        previous = node;
    }
    Edge::create(previous, first)->setType(edgeType);

    return document;
}

void benchmarkOnCircles(const quint32 runSeed)
{
    QRandomGenerator runSeedGenerator(runSeed);
    for (int numberOfNodes = 10; numberOfNodes <= 50; numberOfNodes += 10) {
        GraphDocumentPtr document = generateCircle(numberOfNodes);
        
        LayoutEvaluator evaluator;
        for (int repetition = 0; repetition < REPETITIONS; repetition++) { 
            applyForceBasedLayout(document, runSeedGenerator.generate());
            evaluator.evaluateLayout(document);
        }

        std::cout << "[Force based layout on circles with " << numberOfNodes << " nodes.] {" << std::endl;
        evaluator.showResults(std::cout);
        std::cout << "} " << std::endl << std::endl;
    }
}


GraphDocumentPtr generateRandomTree(const int numberOfNodes, const quint32 seed)
{
    QRandomGenerator randomGenerator(seed);

    Editor editor;
    GraphDocumentPtr document = editor.createDocument();
    EdgeTypePtr edgeType = EdgeType::create(document);
    edgeType->setDirection(EdgeType::Bidirectional);

    for (int i = 0; i < numberOfNodes; i++) {
        NodePtr node = Node::create(document);
        if (i) {
            const int j = randomGenerator.bounded(i);
            Edge::create(document->nodes()[j], node)->setType(edgeType);
        }
    }

    return document;
}

template<typename F> 
void benchmarkOnTrees(const quint32 genSeed, const quint32 runSeed, F applyLayout, const char* name)
{
    QRandomGenerator genSeedGenerator(genSeed);
    QRandomGenerator runSeedGenerator(runSeed);
    for (int numberOfNodes = 10; numberOfNodes <= 50; numberOfNodes += 10) {
        LayoutEvaluator evaluator;
        for (int repetition = 0; repetition < REPETITIONS; repetition++) {
            GraphDocumentPtr document = generateRandomTree(numberOfNodes,
                                                           genSeedGenerator.generate());
            applyLayout(document, runSeedGenerator.generate());
            evaluator.evaluateLayout(document);
        }
    
        std::cout << "[" << name << " on trees with " << numberOfNodes << " nodes.] {" << std::endl;
        evaluator.showResults(std::cout);
        std::cout << "} " << std::endl << std::endl;
    }
}

int main(int argc, char* argv[]) {
    
    //Gets different random number generators seeds for each graph class,
    //making sure that we always generate the same graphs for each class.
    QRandomGenerator seedGenerator(0);
    const quint32 treesGenSeed = seedGenerator.generate();
    
    //Get different random number generator seeds to run the layout algorithms for each class.
    const quint32 pathsRunSeed = seedGenerator.generate();
    const quint32 completeGraphsRunSeed = seedGenerator.generate();
    const quint32 circlesRunSeed = seedGenerator.generate();
    const quint32 treesRunSeed = seedGenerator.generate();

    benchmarkOnPaths(pathsRunSeed, applyForceBasedLayout, "Force based layout");
    benchmarkOnPaths(pathsRunSeed, applyRadialLayout, "Radial layout");
    benchmarkOnCompleteGraphs(completeGraphsRunSeed);
    benchmarkOnCircles(circlesRunSeed);
    benchmarkOnTrees(treesGenSeed, treesRunSeed, applyForceBasedLayout, "Force based layout");
    benchmarkOnTrees(treesGenSeed, treesRunSeed, applyRadialLayout, "Radial layout");
}

