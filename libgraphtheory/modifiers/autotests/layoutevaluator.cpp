#include "layoutevaluator.h"

#include <QLineF>
#include <QVector2D>

void LayoutEvaluator::showMetric(const MetricSummarizer& metric, std::ostream& outputStream) const
{
    outputStream << "\tminimum: " << metric.minimum() << std::endl;
    outputStream << "\tmaximum: " << metric.maximum() << std::endl;
    outputStream << "\taverage: " << metric.average() << std::endl;
}

bool LayoutEvaluator::crosses(const EdgePtr a, const EdgePtr b) const
{
    if (a->from() == b->from() or a->from() == b->to() or 
        a->to() == b->from() or a->to() == b->to()) {

        return false;
    }

    QLineF segmentA(a->from()->x(), a->from()->y(), a->to()->x(), a->to()->y());
    QLineF segmentB(b->from()->x(), b->from()->y(), b->to()->x(), b->to()->y());

    QPointF point;
    return segmentA.intersects(segmentB, &point) == QLineF::BoundedIntersection;
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
