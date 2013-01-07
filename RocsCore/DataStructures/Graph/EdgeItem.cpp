/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "EdgeItem.h"
#include "GraphStructure.h"

#include "NodeItem.h"
#include "Scene/GraphScene.h"
#include "Data.h"
#include "Pointer.h"
#include "DataStructure.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QLine>
#include <QPolygonF>
#include <QtAlgorithms>
#include <KDebug>
#include <QGraphicsSimpleTextItem>
#include <QtCore/qmath.h>
#include <boost/math/constants/constants.hpp>

EdgeItem::EdgeItem(PointerPtr edge, QGraphicsItem *parent)
    : PointerItem(edge, parent)
{
    _loop = (pointer()->from() == pointer()->to());
    setPath(createCurves());
    updatePos(); //TODO this is a workaround: updatePos should be automatically called when
                 //     curves are drawn at Pointer.h; proper fix requires interface changes
                 //     delayed for 4.11

    connect(edge.get(), SIGNAL(changed()), this, SLOT(updatePathLayout()));
    connect(edge.get(), SIGNAL(directionChanged(PointerType::Direction)), this, SLOT(updatePathLayout()));
}

EdgeItem::~EdgeItem()
{
}

void EdgeItem::updatePathLayout()
{
    setPath(createCurves());
}

QPolygonF EdgeItem::createArrow(const QPointF &pos1, const QPointF &pos2) const
{
    const qreal pi = boost::math::constants::pi<double>();
    QLineF line(pos1, pos2);
    qreal angle = ::qAcos(line.dx() / line.length());
    if (line.dy() >= 0) {
        angle = pi*2.0 - angle;
    }
    qreal arrowSize = 10;

    QPointF destArrowP1 = pos2 + QPointF(qSin(angle - pi/3.0) * arrowSize,         qCos(angle - pi/3.0) * arrowSize);
    QPointF destArrowP2 = pos2 + QPointF(qSin(angle - pi + pi/3.0) * arrowSize,    qCos(angle - pi + pi/3.0) * arrowSize);
    QPolygonF arrow(QPolygonF() <<  destArrowP1 << pos2 << destArrowP2);

    /// Put the arrow on the center of the nodes.
    qreal x = pos2.x() - pos1.x();
    qreal y = pos2.y() - pos1.y();

    x -= x / 2;
    y -= y / 2;
    arrow.translate(-x, -y);
    return arrow;
}

QPainterPath EdgeItem::createLoop(const QPointF& pos) const
{
    if (!pointer()) {
        return QPainterPath();
    }
    QPainterPath p;
    DataStructurePtr g = pointer()->dataStructure();
    qreal size = 30 + (20 * pointer()->relativeIndex());
    qreal angle = qAtan2((pos.x() - g->relativeCenter().x()), (pos.y() - g->relativeCenter().y()));
    qreal posx = (pos.x() - (((size / 2) * qSin(angle)) * -1) - (size / 2));
    qreal posy = (pos.y() + (((size / 2) * qCos(angle))) - (size / 2));
    p.addEllipse(posx, posy, size, size);
    return p;
}

QPainterPath EdgeItem::createCurves()
{
    const qreal pi = boost::math::constants::pi<double>();

    Q_ASSERT(pointer());
    if (!pointer()) {
        return QPainterPath();
    }

    if (!pointer()->from() || !pointer()->to()) {
        pointer()->remove();
        return QPainterPath();
    }

    QPointF startPos(pointer()->from()->x(), pointer()->from()->y());

    // test for self loop
    if (_loop) {
        return createLoop(startPos);
    }

    QPointF endPos(pointer()->to()->x(), pointer()->to()->y());

    QPolygonF arrow = createArrow(startPos, endPos);

    if (startPos.x() > endPos.x()) {
        qSwap(startPos, endPos);
    }

    if (pointer()->direction() == PointerType::Bidirectional) {
        QPainterPath p;
        p.moveTo(startPos);
        p.lineTo(endPos);
        return p;
    }

    qreal x = endPos.x() - startPos.x();
    qreal y = endPos.y() - startPos.y();
    qreal angle = atan2(y, x);

    /// Calculate the size of the inclination on the curve.
    qreal theta = angle + pi/2.0;
    qreal finalX = qCos(theta);
    qreal finalY = qSin(theta);
    int lastIndex = pointer()->relativeIndex();

    if (lastIndex & 1) {   // If the number is odd.
        ++lastIndex;
        finalX *= (-1);
        finalY *= (-1);
    }

    qreal size = qSqrt(qPow((x * 0.1), 2) + qPow((y * 0.1), 2)) * lastIndex;

    finalX *= size;
    finalY *= size;
    finalX += startPos.x() + x / 2;
    finalY += startPos.y() + y / 2;

    /// Draw the Arc.
    QPainterPath p;
    p.moveTo(startPos);
    p.quadTo(finalX, finalY, endPos.x(), endPos.y());

    /// puts the arrow on its correct position
    QPointF middle = p.pointAtPercent(0.5);

    x = startPos.x() + (endPos.x() - startPos.x()) / 2;
    y = startPos.y() + (endPos.y() - startPos.y()) / 2;
    QLineF line2(QPointF(x, y), middle);

    arrow.translate(+line2.dx(), +line2.dy());
    p.addPolygon(arrow);

    return p;
}

#include "EdgeItem.moc"
