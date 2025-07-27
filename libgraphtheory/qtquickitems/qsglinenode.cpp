/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "qsglinenode.h"
#include "qsgarrowheadnode.h"
#include <QColor>
#include <QSGGeometry>

using namespace GraphTheory;

QSGLineNode::QSGLineNode()
    : m_geometry(QSGGeometry::defaultAttributes_Point2D(), 2)
    , m_arrowHead(nullptr)
{
    m_material.setColor(QColor(0, 0, 0));
    setMaterial(&m_material);

    setGeometry(&m_geometry);
    m_geometry.setDrawingMode(QSGGeometry::DrawLines);
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

void QSGLineNode::setColor(const QColor &color)
{
    QSGFlatColorMaterial *m = static_cast<QSGFlatColorMaterial *>(material());
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
        m_arrowHead = nullptr;
    }
}
