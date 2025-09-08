/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef QSGLINENODE_H
#define QSGLINENODE_H

#include "edgetype.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

class QColor;

namespace GraphTheory
{
class QSGArrowHeadNode;

class QSGLineNode : public QSGGeometryNode
{
public:
    explicit QSGLineNode();
    ~QSGLineNode() override;
    void setLine(const QPointF &from, const QPointF &to);
    void setColor(const QColor &color);
    void setDirection(EdgeType::Direction direction);

private:
    QSGGeometry m_geometry;
    QSGFlatColorMaterial m_material;
    QPointF m_from;
    QPointF m_to;
    GraphTheory::QSGArrowHeadNode *m_arrowHead;
};
}

#endif
