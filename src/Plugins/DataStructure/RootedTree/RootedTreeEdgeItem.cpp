/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#include "RootedTreeEdgeItem.h"
#include "RootedTreeStructure.h"
#include "RootedTreeNodeItem.h"

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
#include "RootedTreeNode.h"


static const qreal Pi = 3.14159265358979323846264338327950288419717;

/// The value of Pi * 2
static const qreal TwoPi = 2.0 * Pi;

/// The Value of PI / 3
static const qreal PI_3 = Pi / 3.0;

/// Arrow size
static const qreal ArrowSize = 10.0;

RootedTreeEdgeItem::RootedTreeEdgeItem( PointerPtr edge, QGraphicsItem *parent)
        : PointerItem(edge, parent)
{
    connect (edge.get(), SIGNAL(changed()), this, SLOT(updatePathLayout()));
//     setPath(createCurves());
}

RootedTreeEdgeItem::~RootedTreeEdgeItem() {
//  dynamic_cast<GraphScene*>(scene())->removeGItem(this);
}

void RootedTreeEdgeItem::updatePathLayout() {
//     setPath(createCurves());
    update();
}

QPointF RootedTreeEdgeItem::startPoint()
{
    m_startPoint = QPointF();
    if (!pointer()->dataStructure()->property("ShowAllPointers").toBool()){
        return QPointF(pointer()->from()->x(), pointer()->from()->y());
    }
    if (scene()){
        if(QGraphicsItem * item = scene()->itemAt(pointer()->from()->x(), pointer()->from()->y())){
            if ((pointer()->property("TreeEdge").isValid())){
		RootedTreeNode* fromNode = qobject_cast<RootedTreeNode*>(pointer()->from().get());
                const qreal pointersSize = pointer()->dataStructure()->property("PointersRegion").toReal();
                qint16 childCount = fromNode->numberOfChilds();
                const qreal size = pointer()->dataStructure()->property("NodeSize").toReal();
                int p = pointer()->property("TreeEdge").toInt();
                if (p < 0)
                    m_startPoint =  item->mapToScene(QPointF(size/2,
                                            size * (pointersSize/2))) ;//Draw as Parent
                else{
                    const qreal division = ((childCount * pointersSize) > 1 ?
                                            childCount * pointersSize * size:
                                            size) / childCount;
                    m_startPoint = item->mapToScene(QPointF(division * (p + 0.5), size * ( 1+ pointersSize + pointersSize/2) ) );
                }
            }
        }
        else
            m_startPoint = QPointF(pointer()->from()->x(),pointer()->from()->y());
    }

    m_endPoint = endPoint();
    return m_startPoint;
}

QPointF RootedTreeEdgeItem::endPoint() const
{
//     if (pointer()->from().get() || pointer()->to().get()) {
    if (!pointer()->dataStructure()->property("ShowAllPointers").toBool()){
        return QPointF(pointer()->to()->x(), pointer()->to()->y());
    }
    const qreal size = pointer()->dataStructure()->property("NodeSize").toReal();
    if(QGraphicsItem * item = scene()->itemAt(pointer()->to()->x(), pointer()->to()->y())) {
        if (pointer()->property("TreeEdge").isValid() && pointer()->property("TreeEdge").toInt() != -1)
            return item->mapToScene( size/2, -size * 0.05);
        else if (RootedTreeNode* parentNode = qobject_cast<RootedTreeNode*>(pointer()->to().get())) {
            for(quint32 count = 0; count <= parentNode->numberOfChilds(); ++count) {
                if (parentNode->child(count).get() &&  parentNode->child(count).get() == pointer()->from().get())
                    return item->mapToScene( (count+1) * size/parentNode->numberOfChilds() , size*1.65);
            }
            return item->mapToScene( size/2 , size*1.65);
        }
    }
//     }
    return QPointF();
}


void RootedTreeEdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
    if (pointer()->to() && pointer()->from() ) {
        m_startPoint = startPoint();
        m_endPoint = endPoint();
        QLineF line(m_startPoint, m_endPoint);
        if (!m_startPoint.isNull()) {
            
            if (!pointer()->dataStructure()->property("ShowAllPointers").toBool())
                if (pointer()->property("TreeEdge").toInt() == -1)
                    return;
            
            painter->drawLine(line);

            qreal angle = acos(line.dx() / line.length());
            if (line.dy() >= 0)
                angle = TwoPi - angle;
            QPointF arrowP1 = m_endPoint + QPointF(sin(angle - PI_3) * ArrowSize,
                                                   cos(angle - PI_3) * ArrowSize);
            QPointF arrowP2 = m_endPoint + QPointF(sin(angle - Pi + PI_3) * ArrowSize,
                                                   cos(angle - Pi + PI_3) * ArrowSize);

            painter->setBrush(QBrush(Qt::black,Qt::SolidPattern));
            painter->drawEllipse(m_startPoint, 3, 3);
            painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPolygon(QPolygonF() << m_endPoint << arrowP1 << arrowP2);
        }
    }

}

QRectF RootedTreeEdgeItem::boundingRect() const
{
    if (pointer()->to().get() && pointer()->from().get() && pointer()->property("TreeEdge").toInt() != -1) {
        if (!m_startPoint.isNull()) {
            qreal penWidth = 1;
            qreal extra = (penWidth + ArrowSize) / 2.0;
            return QRectF(m_startPoint, m_endPoint).normalized().adjusted(-extra, -extra, extra, extra);
        }
    }
    return QRectF();

}


#include "RootedTreeEdgeItem.moc"
