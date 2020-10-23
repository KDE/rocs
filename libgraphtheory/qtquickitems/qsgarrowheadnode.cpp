/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "qsgarrowheadnode.h"
#include <QSGGeometry>
#include <QColor>

using namespace GraphTheory;

QSGArrowHeadNode::QSGArrowHeadNode()
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 3)
{
    m_material.setColor(QColor(0, 0, 0));
    setMaterial(&m_material);

    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(GL_TRIANGLES);
}

QSGArrowHeadNode::~QSGArrowHeadNode()
{

}

void QSGArrowHeadNode::setArrow(const QPointF &from, const QPointF &to)
{
    const qreal baseSize = 6;
    const qreal padding = 8; // distance from head to node center

    // compute main axis of head
    QVector2D normale(to - from);
    normale.normalize();
    normale = normale * baseSize;

    // compute orthogonal line: switch x and y coefficients and change the sign of one
    QVector2D halfBaseLine(normale.y(), -normale.x());

    // make computations obvious
    const QVector2D paddingVec(normale * padding / normale.length());
    const QPointF A(to - paddingVec.toPointF());
    const QPointF B(to - paddingVec.toPointF() - 3 * normale.toPointF() + halfBaseLine.toPointF());
    const QPointF C(to - paddingVec.toPointF() - 3 * normale.toPointF() - halfBaseLine.toPointF());

    m_geometry.vertexDataAsPoint2D()[0].set(A.x(), A.y()); // pointy end
    m_geometry.vertexDataAsPoint2D()[1].set(B.x(), B.y()); // left bottom
    m_geometry.vertexDataAsPoint2D()[2].set(C.x(), C.y()); // right bottom

    markDirty(QSGNode::DirtyGeometry);
}

void QSGArrowHeadNode::setColor(const QColor& color)
{
    QSGFlatColorMaterial *m = static_cast<QSGFlatColorMaterial*>(material());
    m->setColor(color);
    markDirty(QSGNode::DirtyMaterial);
}
