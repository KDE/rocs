/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "zoom.h"
#include <GraphScene.h>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QDebug>
#include <KLocale>
#include <QGraphicsSceneWheelEvent>

ZoomAction::ZoomAction(GraphScene* scene, QObject* parent)
: AbstractAction(scene, parent)
,m_view(scene->views().at(0))
,m_zoomRectItem(0)
{
    setText(i18n ( "Zoom" ));
    setToolTip ( i18n ( "Zoom the canvas by the wheel, or by dragging." ) );
    setIcon ( KIcon ( "page-zoom" ) );
    _name = "zoom";
}

bool ZoomAction::executePress(QPointF pos)
{
    delete m_zoomRectItem;
    
    m_zoomRectItem = new QGraphicsRectItem(0,0,0,0);
    QColor color(Qt::green);
    color.setAlphaF(0.3);
    m_zoomRectItem->setBrush(QBrush(color));
    m_zoomRectItem->setPen(QPen(QBrush(Qt::black), 1, Qt::SolidLine));
    m_zoomRectItem->setZValue(9);
    m_beginZoom = m_view->mapFromScene(pos);
    
    _graphScene->addItem(m_zoomRectItem);
    return true;
}

bool ZoomAction::executeMove(QPointF pos)
{
    if (!m_zoomRectItem)
        return false;
    
    QPoint movePos = m_view->mapFromScene(pos);
    qreal left   = (m_beginZoom.x() < movePos.x()) ? m_beginZoom.x() : movePos.x();
    qreal top    = (m_beginZoom.y() < movePos.y()) ? m_beginZoom.y() : movePos.y();
    qreal bottom = (m_beginZoom.y() > movePos.y()) ? m_beginZoom.y() : movePos.y();
    qreal right  = (m_beginZoom.x() > movePos.x()) ? m_beginZoom.x() : movePos.x();
    
    QPointF topLeft(m_view->mapToScene(left, top));
    QPointF bottomRight(m_view->mapToScene(right, bottom));
    
    m_zoomRectItem->setRect(QRectF(topLeft, bottomRight));
    
    return true;
}

bool ZoomAction::executeRelease(QPointF pos)
{
    if (!m_zoomRectItem)
        return false;
    
    qreal left   = (m_beginZoom.x() < pos.x()) ? m_beginZoom.x() : pos.x(); 
    qreal top    = (m_beginZoom.y() < pos.y()) ? m_beginZoom.y() : pos.y(); 
    qreal bottom = (m_beginZoom.x() > pos.x()) ? m_beginZoom.x() : pos.x(); 
    qreal right  = (m_beginZoom.y() > pos.y()) ? m_beginZoom.y() : pos.y();
    
    qDebug() << "top" << top << "left" << left << "bottom" << bottom << "right" << right;
    delete m_zoomRectItem;
    m_zoomRectItem = 0;
    return true;
}

bool ZoomAction::executeDoubleClick(QPointF pos)
{
    return AbstractAction::executeDoubleClick(pos);
}

bool ZoomAction::executeKeyPress(QKeyEvent* keyEvent)
{
    return AbstractAction::executeKeyPress(keyEvent);
}

bool ZoomAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    return AbstractAction::executeKeyRelease(keyEvent);
}

bool ZoomAction::executeWellEvent(QGraphicsSceneWheelEvent* wEvent)
{
    if (wEvent->delta() > 0){
        m_view->scale(1.25, 1.25);
    }else{
        m_view->scale(1/1.25, 1/1.25);
    }
    return true;
}



