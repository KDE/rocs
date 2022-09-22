/*
 *  SPDX-FileCopyrightText: 2011-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef TRANSFORMEDGESWIDGET_H
#define TRANSFORMEDGESWIDGET_H

#include "typenames.h"
#include "ui_transformedgeswidget.h"
#include <QDialog>

namespace GraphTheory
{

class TransformEdgesWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TransformEdgesWidget(GraphDocumentPtr document, QWidget *parent = nullptr);
    ~TransformEdgesWidget() override;

public Q_SLOTS:
    void transform();

private:
    /**
     * Create edges between all nodes.
     */
    void makeComplete();

    /**
     * Remove all edges.
     */
    void removeAllEdges();

    /**
     * Reverse all existing edges.
     */
    void reverseAllEdges();

    /**
     * Transform given graph to a spanning tree by executing Prim's minimum spanning tree (MST)
     * algorithm. All edges are assumed to have weight 1 if no weights are given. Otherwise,
     * with given weights the specified values are used.
     *
     * \return  total weight of MST
     */
    qreal makeSpanningTree();

    /**
     * Remove all self-edges.
     */
    void removeAllSelfEdges();

    GraphDocumentPtr m_document;
    Ui::TransformEdgesWidget *ui;
};
}

#endif
