/*
 *  SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
