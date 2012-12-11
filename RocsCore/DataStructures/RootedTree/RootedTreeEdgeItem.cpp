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
#include <boost/math/constants/constants.hpp>


static const qreal ArrowSize = 10.0;

RootedTreeEdgeItem::RootedTreeEdgeItem(PointerPtr edge, QGraphicsItem *parent)
    : PointerItem(edge, parent)
{
    connect (edge.get(), SIGNAL(changed()), this,
             SLOT(updatePathLayout()));
    connect (edge->to().get(), SIGNAL(widthChanged(double)), this, SLOT(updatePathLayout()));
    connect (edge->from().get(), SIGNAL(widthChanged(double)), this, SLOT(updatePathLayout()));
    QTimer::singleShot(50, this, SLOT(updatePathLayout()));
}

RootedTreeEdgeItem::~RootedTreeEdgeItem() {
}

void RootedTreeEdgeItem::updatePathLayout() {
    setPath(createCurves());
    update();
}

QPointF RootedTreeEdgeItem::startPoint()
{
    if (pointer()->dataStructure()->property("ShowAllPointers").isValid() && pointer()->dataStructure()->property("ShowAllPointers").toBool()){
        if (scene()){
            RootedTreeNodeItem * item = 0;
            foreach(QGraphicsItem * i, scene()->items(QPointF(pointer()->from()->x(), pointer()->from()->y()))){
                if (qgraphicsitem_cast<RootedTreeNodeItem*>(i)){
                    item = qgraphicsitem_cast<RootedTreeNodeItem*>(i);
                }
            }
            if(item) {
                if ((pointer()->property("TreeEdge").isValid())){
                    RootedTreeNode* fromNode = qobject_cast<RootedTreeNode*>(pointer()->from().get());
                    const qreal pointersSize = pointer()->dataStructure()->property("PointersRegion").toReal();
                    qint16 childCount = fromNode->numberOfChilds();
                    const qreal size = pointer()->dataStructure()->property("NodeSize").toReal();
                    int p = pointer()->property("TreeEdge").toInt();
                    if (p < 0)
                        return item->mapToScene( QPointF(size/2 ,
                                                size * (pointersSize/2) )) ;//Draw as Parent
                    else{
                        const qreal division = ((childCount * pointersSize) > 1 ?
                                                                            childCount * pointersSize * size:
                                                                            size) / childCount;
                        return item->mapToScene(QPointF(division * (p + 0.5),
                                       size * ( 1+ pointersSize + pointersSize/2)) );
                    }
                }
            }
        }
    }
    return QPointF(pointer()->from()->x(),pointer()->from()->y());
}


QPointF RootedTreeEdgeItem::endPoint() const
{
    if (pointer()->dataStructure()->property("ShowAllPointers").isValid() && pointer()->dataStructure()->property("ShowAllPointers").toBool()){

        const qreal size = pointer()->dataStructure()->property("NodeSize").toReal();
        if (scene()){
            RootedTreeNodeItem * item = 0;
            foreach(QGraphicsItem * i, scene()->items(QPointF(pointer()->to()->x(), pointer()->to()->y()))){
                if (qgraphicsitem_cast<RootedTreeNodeItem*>(i)){
                    item = qgraphicsitem_cast<RootedTreeNodeItem*>(i);
                }
            }
            if(item) {
                if (pointer()->property("TreeEdge").isValid() && pointer()->property("TreeEdge").toInt() != -1) {
                    return item->mapToScene( size/2, -size * 0.05);
                } else if (RootedTreeNode* parentNode = qobject_cast<RootedTreeNode*>(pointer()->to().get())) {
                    for(quint32 count = 0; count <= parentNode->numberOfChilds(); ++count) {
                        if (parentNode->child(count).get() &&  parentNode->child(count).get() == pointer()->from().get()) {
                            return item->mapToScene( (count+1) * size/parentNode->numberOfChilds() , size*1.65);
                        }
                    }
                    return item->mapToScene( size/2 , size*1.65);
                }
            }
        }
    }
    return QPointF(pointer()->to()->x(), pointer()->to()->y());;
}

QPainterPath RootedTreeEdgeItem::createCurves()
{
    const qreal pi = boost::math::constants::pi<double>();

    QPainterPath painter;
    if (pointer()->to() && pointer()->from()) {
        QPointF m_startPoint = startPoint();
        QPointF m_endPoint = endPoint();
        QLineF line(m_startPoint, m_endPoint);
        if (!m_startPoint.isNull()) {

            if (!pointer()->dataStructure()->property("ShowAllPointers").toBool()) {
                if (pointer()->property("TreeEdge").toInt() == -1) {
                    return QPainterPath();
                }
            }
            painter.moveTo(line.p1());
            painter.lineTo(line.p2());

            qreal angle = acos(line.dx() / line.length());
            if (line.dy() >= 0){
                angle =  pi*2.0 - angle;
            }
            QPointF arrowP1 = m_endPoint + QPointF(sin(angle - pi/3.0) * ArrowSize,
                                                   cos(angle - pi/3.0) * ArrowSize);
            QPointF arrowP2 = m_endPoint + QPointF(sin(angle - pi + pi/3.0) * ArrowSize,
                                                   cos(angle - pi + pi/3.0) * ArrowSize);


            painter.addEllipse(m_startPoint, 3, 3);

            painter.addPolygon(QPolygonF() << m_endPoint << arrowP1 << arrowP2 << m_endPoint);
        }
    }
    return painter;
}


#include "RootedTreeEdgeItem.moc"
