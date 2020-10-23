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

#include "qsglinenode.h"
#include "qsgarrowheadnode.h"
#include <QSGGeometry>
#include <QColor>

using namespace GraphTheory;

QSGLineNode::QSGLineNode()
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 2)
    , m_arrowHead(0)
{
    m_material.setColor(QColor(0, 0, 0));
    setMaterial(&m_material);

    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(GL_LINES);
    m_geometry.setLineWidth(2);
}

QSGLineNode::~QSGLineNode()
{
    delete m_arrowHead;
}

void QSGLineNode::setLine(const QPointF &from, const QPointF &to)
{
    m_from = from;
    m_to = to;
    m_geometry.vertexDataAsPoint2D()[0].set(from.x(), from.y());
    m_geometry.vertexDataAsPoint2D()[1].set(to.x(), to.y());
    if (m_arrowHead) {
        m_arrowHead->setArrow(from, to);
    }
    markDirty(QSGNode::DirtyGeometry);
}

void QSGLineNode::setColor(const QColor& color)
{
    QSGFlatColorMaterial *m = static_cast<QSGFlatColorMaterial*>(material());
    m->setColor(color);
    if (m_arrowHead) {
        m_arrowHead->setColor(color);
    }
    markDirty(QSGNode::DirtyMaterial);
}

void QSGLineNode::setDirection(EdgeType::Direction direction)
{
    if (!m_arrowHead && direction == EdgeType::Unidirectional) {
        m_arrowHead = new QSGArrowHeadNode;
        m_arrowHead->setArrow(m_from, m_to);
        appendChildNode(m_arrowHead);
    }
    if (m_arrowHead && direction == EdgeType::Bidirectional) {
        removeChildNode(m_arrowHead);
        delete m_arrowHead;
        m_arrowHead = 0;
    }
}
