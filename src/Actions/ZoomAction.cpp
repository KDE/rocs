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


#include "ZoomAction.h"
#include <GraphScene.h>
#include <DocumentManager.h>
#include <Document.h>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QDebug>
#include <KLocale>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>

qreal ZoomAction::_zoomFactor = 1;

ZoomAction::ZoomAction(GraphScene* scene, QObject* parent)
    : AbstractAction(scene, parent)
    , m_view(scene->views().at(0))
    , m_zoomRectItem(0)
{
    setText(i18nc("@action:intoolbar", "Zoom"));
    setToolTip(i18nc("@info:tooltip", "Zoom the canvas by the wheel, or by dragging."));
    setIcon(KIcon("page-zoom"));
    _name = "zoom";
}

bool ZoomAction::executePress(QPointF pos)
{
    delete m_zoomRectItem;

    m_zoomRectItem = new QGraphicsRectItem(0, 0, 0, 0);
    QColor color(Qt::green);
    color.setAlphaF(0.3);
    m_zoomRectItem->setBrush(QBrush(color));
    m_zoomRectItem->setPen(QPen(QBrush(Qt::black), 0.1, Qt::SolidLine));
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

bool ZoomAction::executeRelease(QPointF)
{
    if (!m_zoomRectItem)
        return false;

    m_view->fitInView(m_zoomRectItem->rect(), Qt::KeepAspectRatioByExpanding);
    delete m_zoomRectItem;
    m_zoomRectItem = 0;
    return true;
}

bool ZoomAction::executeDoubleClick(QPointF)
{
    zoomReset();
    return true;
}

bool ZoomAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_Plus  : {
        m_view->scale(1.25, 1.25);
        _zoomFactor *= 1.25;
        break;
    }
    case Qt::Key_Minus : {
        m_view->scale(0.8, 0.8);
        _zoomFactor *= 0.8;
        break;
    }
    case Qt::Key_5     : m_view->resetMatrix();     break;
    }
    keyEvent->accept();
    return true;
}

bool ZoomAction::executeWheelEvent(QGraphicsSceneWheelEvent* wEvent)
{
    if (wEvent->delta() < 0) { // zoom out
        zoomOut(wEvent->scenePos());
    } else { // zoom in
        zoomIn(wEvent->scenePos());
    }
//     qDebug() << "event position: "<< wEvent->scenePos();
    wEvent->accept();
    return true;
}

void ZoomAction::zoomInCenter()
{
    m_view->scale(1.25, 1.25);
    _zoomFactor *= 1.25;
}


void ZoomAction::zoomIn(QPointF zoomCenter)
{
    zoomInCenter();
    m_view->centerOn(zoomCenter);
}

void ZoomAction::zoomOutCenter()
{
    if (
        m_view->width() / _zoomFactor * 1.25 < 5 * DocumentManager::self()->activeDocument()->width()
        || m_view->height() / _zoomFactor * 1.25 < 5 * DocumentManager::self()->activeDocument()->height()
    ) {
        m_view->scale(0.8, 0.8);
        _zoomFactor *= 0.8;
    }
}


void ZoomAction::zoomOut(QPointF zoomCenter)
{
    zoomOutCenter();
    m_view->centerOn(zoomCenter);
}

void ZoomAction::zoomReset()
{
    m_view->resetMatrix();
}
