// SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
// SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL

#ifndef QSGARROWHEADNODE_H
#define QSGARROWHEADNODE_H

#include <QSGFlatColorMaterial>
#include <QSGGeometryNode>

class QColor;

namespace GraphTheory
{
class QSGArrowHeadNode : public QSGGeometryNode
{
public:
    explicit QSGArrowHeadNode();
    ~QSGArrowHeadNode() override;
    void setArrow(const QPointF &from, const QPointF &to);
    void setColor(const QColor &color);

private:
    QSGGeometry m_geometry;
    QSGFlatColorMaterial m_material;
};
}

#endif
