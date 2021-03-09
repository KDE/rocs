/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "layoutevaluator.h"

#include <QLineF>
#include <QVector2D>
#include <iostream>
#include <iomanip>

void LayoutEvaluator::showMetric(const MetricSummarizer& metric, std::ostream& outputStream) const
{
    outputStream << "\tminimum: " << metric.minimum() << std::endl;
    outputStream << "\tmaximum: " << metric.maximum() << std::endl;
    outputStream << "\taverage: " << metric.average() << std::endl;
}

QPointF LayoutEvaluator::projectOntoSegment(const QLineF& segment, const QPointF& point) const
{
    const QPointF p1p2 = segment.p1() - segment.p2();
    const QPointF pointp2 = point - segment.p2();
    const qreal denominator = QPointF::dotProduct(p1p2, p1p2);
    const qreal numerator = QPointF::dotProduct(p1p2, pointp2);
    const qreal lambda = qMin(1., qMax(0., numerator / denominator));
    return segment.p1() * lambda + segment.p2() * (1. - lambda);
}

qreal LayoutEvaluator::squaredDistance(const QPointF& pointA, const QPointF& pointB) const
{
    return QPointF::dotProduct(pointA - pointB, pointA - pointB);
}

bool LayoutEvaluator::doSegmentsIntersect(const QLineF& segmentA, const QLineF& segmentB) const
{
    constexpr qreal EPS = 1.e-1;

    QPointF point;
    if (segmentA.intersects(segmentB, &point) != QLineF::BoundedIntersection) {
        return false;
    }

    if (squaredDistance(point, projectOntoSegment(segmentA, point)) > EPS) {
        return false;
    }

    if (squaredDistance(point, projectOntoSegment(segmentB, point)) > EPS) {
        return false;
    }

    return true;
}

bool LayoutEvaluator::crosses(const EdgePtr a, const EdgePtr b) const
{
    if (a->from() == b->from() or a->from() == b->to() or 
        a->to() == b->from() or a->to() == b->to()) {

        return false;
    }

    QPointF aFrom(a->from()->x(), a->from()->y());
    QPointF aTo(a->to()->x(), a->to()->y());
    QPointF bFrom(b->from()->x(), b->from()->y());
    QPointF bTo(b->to()->x(), b->to()->y());

    QLineF segmentA(aFrom, aTo);
    QLineF segmentB(bFrom, bTo);
    
    return doSegmentsIntersect(segmentA, segmentB);
}

int LayoutEvaluator::calculateNumberOfEdgeCrosses(GraphDocumentPtr document)
{
    int numberOfEdgeCrosses = 0;
    for (const EdgePtr edgeA : document->edges()) {
        for (const EdgePtr edgeB : document->edges()) {
            if (edgeA != edgeB and crosses(edgeA, edgeB)) {
                numberOfEdgeCrosses++;
            }
        }
    }
    return numberOfEdgeCrosses / 2;
}

int LayoutEvaluator::calculateNumberOfEdgesWithCrosses(GraphDocumentPtr document)
{
    int numberOfEdgesWithCrosses = 0;
    for (const EdgePtr edgeA : document->edges()) {
        for (const EdgePtr edgeB : document->edges()) {
            if (edgeA != edgeB and crosses(edgeA, edgeB)) {
                numberOfEdgesWithCrosses++;
                break;
            }
        }
    } 
    return numberOfEdgesWithCrosses;
}

bool LayoutEvaluator::intersects(const NodePtr a, const NodePtr b) const
{
    constexpr qreal EPS = 1.e-4;
    constexpr qreal nodeRadius = 10.;
    const QPointF pA(a->x(), a->y());
    const QPointF pB(b->x(), b->y());
    const QVector2D distance(pA - pB);
    return distance.length() <  2. * nodeRadius - EPS;
}


int LayoutEvaluator::calculateNumberOfNodeIntersections(GraphDocumentPtr document)
{
    int numberOfNodeIntersections = 0;
    for (const NodePtr nodeA : document->nodes()) {
        for (const NodePtr nodeB : document->nodes()) {
            if (nodeA != nodeB and intersects(nodeA, nodeB)) {
                numberOfNodeIntersections++;
            }
        }
    }
    return numberOfNodeIntersections / 2;
}

int LayoutEvaluator::calculateNumberOfNodesWithIntersections(GraphDocumentPtr document)
{
    int numberOfNodesWithIntersections = 0;
    for (const NodePtr nodeA : document->nodes()) {
        for (const NodePtr nodeB : document->nodes()) {
            if (nodeA != nodeB and intersects(nodeA, nodeB)) {
                numberOfNodesWithIntersections++;
                break;
            }
        }
    }
    return numberOfNodesWithIntersections;
}

void LayoutEvaluator::evaluateLayout(GraphDocumentPtr document)
{
    numberOfEdgeCrossesMetric_m.addValue(calculateNumberOfEdgeCrosses(document));
    
    numberOfEdgesWithCrossesMetric_m.addValue(calculateNumberOfEdgesWithCrosses(document));
    
    numberOfNodeIntersectionsMetric_m.addValue(calculateNumberOfNodeIntersections(document));
    
    numberOfNodesWithIntersectionsMetric_m.addValue(calculateNumberOfNodesWithIntersections(
        document));
}

void LayoutEvaluator::showResults(std::ostream& outputStream) const
{
    outputStream << "Number of edge crosses:" << std::endl;
    showMetric(numberOfEdgeCrossesMetric_m, outputStream);
    outputStream << std::endl;

    outputStream << "Number of edges with crosses:" << std::endl;
    showMetric(numberOfEdgesWithCrossesMetric_m, outputStream);
    outputStream << std::endl;

    outputStream << "Number of node intersections:" << std::endl;
    showMetric(numberOfNodeIntersectionsMetric_m, outputStream);
    outputStream << std::endl;

    outputStream << "Number of nodes with intersections:" << std::endl;
    showMetric(numberOfNodesWithIntersectionsMetric_m, outputStream);
    outputStream << std::endl;
}
