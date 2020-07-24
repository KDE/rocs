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

#ifndef LAYOUTEVALUATOR_H
#define LAYOUTEVALUATOR_H

#include <QObject>
#include <QString>
#include <QVector>
#include <iostream>
#include "metricsummarizer.h"
#include "graphdocument.h"
#include "editor.h"
#include "edge.h"
#include "node.h"

using namespace GraphTheory;

/*
 * Evaluates graph layouts with respect to some aesthetic criteria.
 */
class LayoutEvaluator : public QObject
{
    Q_OBJECT;
private:
    MetricSummarizer numberOfEdgeCrossesMetric_m;
    MetricSummarizer numberOfEdgesWithCrossesMetric_m;
    MetricSummarizer numberOfNodeIntersectionsMetric_m;
    MetricSummarizer numberOfNodesWithIntersectionsMetric_m;

    void showMetric(const MetricSummarizer& metric, std::ostream& outputStream) const;
    
    bool crosses(const EdgePtr a, const EdgePtr b) const;
    int calculateNumberOfEdgeCrosses(GraphDocumentPtr document);
    int calculateNumberOfEdgesWithCrosses(GraphDocumentPtr document);

    bool intersects(const NodePtr a, const NodePtr b) const;
    int calculateNumberOfNodeIntersections(GraphDocumentPtr document);
    int calculateNumberOfNodesWithIntersections(GraphDocumentPtr document);
    QPointF projectOntoSegment(const QLineF& segment, const QPointF& point) const;
    qreal squaredDistance(const QPointF& pointA, const QPointF& pointB) const;
    bool doSegmentsIntersect(const QLineF& segmentA, const QLineF& segmentB) const;
public:

    /*
     * Evaluates a new layout.
     */
    void evaluateLayout(GraphDocumentPtr document);

    /*
     * Presents a summary of each metric, combining the results of all layouts evaluated.
     */
    void showResults(std::ostream& outputStream) const;
};

 #endif
