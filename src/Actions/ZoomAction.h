/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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


#ifndef ZOOMACTION_H
#define ZOOMACTION_H

#include <AbstractAction.h>
#include <QRectF>
#include <QPointF>

class QGraphicsRectItem;
class QGraphicsView;
class QKeyEvent;
class QGraphicsSceneWheelEvent;

class ZoomAction : public AbstractAction
{
    Q_OBJECT

public:
    ZoomAction(GraphScene* scene, QObject* parent);
    bool executePress(QPointF pos);
    bool executeMove(QPointF pos);
    bool executeRelease(QPointF pos);
    bool executeDoubleClick(QPointF pos);
    bool executeKeyRelease(QKeyEvent* keyEvent);
    bool executeWheelEvent(QGraphicsSceneWheelEvent *wEvent);
    
public slots:
    void zoomOut(QPointF zoomCenter);
    void zoomIn(QPointF zoomCenter);
    void zoomInCenter();
    void zoomOutCenter();
    void zoomReset();
    
private:
    QRectF m_zoomArea;
    qreal m_currentZoomFactor;
    QGraphicsView *m_view;
    QGraphicsRectItem *m_zoomRectItem;
    QPointF m_beginZoom;
    QPointF m_endZoom;
    int m_steps;
      
    static qreal _zoomFactor;
};

#endif // ZOOM_H
