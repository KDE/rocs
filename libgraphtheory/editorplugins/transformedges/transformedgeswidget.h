/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#ifndef TRANSFORMEDGESWIDGET_H
#define TRANSFORMEDGESWIDGET_H

#include "ui_transformedgeswidget.h"
#include "typenames.h"
#include <QDialog>

namespace GraphTheory {

class TransformEdgesWidget : public QDialog
{
    Q_OBJECT

public:
    explicit TransformEdgesWidget(GraphDocumentPtr document, QWidget *parent = 0);
    ~TransformEdgesWidget();

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


    GraphDocumentPtr m_document;
    Ui::TransformEdgesWidget *ui;
};
}

#endif
