/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <Scene/GraphScene.h>
#include <GraphVisualEditor.h>
#include <DocumentManager.h>
#include <Document.h>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QDebug>
#include <KLocalizedString>
#include <QIcon>
#include <QGraphicsSceneWheelEvent>
#include <QKeyEvent>

ZoomAction::ZoomAction(GraphScene* scene, QObject* parent)
    : AbstractAction(scene, parent)
    , _scene(scene)
    , m_zoomRectItem(0)
{
    setText(i18nc("@action:intoolbar", "Zoom"));
    setToolTip(i18nc("@info:tooltip", "Zoom the canvas by the wheel, or by dragging."));
    setIcon(QIcon::fromTheme("page-zoom"));
    _name = "zoom";
}

bool ZoomAction::executePress(QPointF pos)
{
    delete m_zoomRectItem;
    QGraphicsView *view = _scene->views().at(0);

    m_zoomRectItem = new QGraphicsRectItem(0, 0, 0, 0);
    QColor color(Qt::green);
    color.setAlphaF(0.3);
    m_zoomRectItem->setBrush(QBrush(color));
    m_zoomRectItem->setPen(QPen(QBrush(Qt::black), 0.1, Qt::SolidLine));
    m_zoomRectItem->setZValue(9);
    m_beginZoom = view->mapFromScene(pos);

    _graphScene->addItem(m_zoomRectItem);
    return true;
}

bool ZoomAction::executeMove(QPointF pos)
{
    QGraphicsView *view = _scene->views().at(0);
    if (!m_zoomRectItem) {
        return false;
    }

    QPoint movePos = view->mapFromScene(pos);
    qreal left   = (m_beginZoom.x() < movePos.x()) ? m_beginZoom.x() : movePos.x();
    qreal top    = (m_beginZoom.y() < movePos.y()) ? m_beginZoom.y() : movePos.y();
    qreal bottom = (m_beginZoom.y() > movePos.y()) ? m_beginZoom.y() : movePos.y();
    qreal right  = (m_beginZoom.x() > movePos.x()) ? m_beginZoom.x() : movePos.x();

    QPointF topLeft(view->mapToScene(left, top));
    QPointF bottomRight(view->mapToScene(right, bottom));

    m_zoomRectItem->setRect(QRectF(topLeft, bottomRight));

    return true;
}

bool ZoomAction::executeRelease(QPointF)
{
    if (!m_zoomRectItem) {
        return false;
    }
    _scene->zoomToRect(m_zoomRectItem->rect());
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
    case Qt::Key_Plus : {
        _scene->zoomBy(1.25);
        break;
    }
    case Qt::Key_Minus : {
        _scene->zoomBy(0.8);
        break;
    }
    case Qt::Key_5 : {
        _scene->resetZoom();
        break;
    }
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
    wEvent->accept();
    return true;
}

void ZoomAction::zoomInCenter()
{
    _scene->zoomBy(1.25);
}

void ZoomAction::zoomIn(QPointF zoomCenter)
{
    zoomInCenter();
    _scene->centerOn(zoomCenter);
}

void ZoomAction::zoomOutCenter()
{
    _scene->zoomBy(0.8);

}

void ZoomAction::zoomOut(QPointF zoomCenter)
{
    zoomOutCenter();
    _scene->centerOn(zoomCenter);
}

void ZoomAction::zoomReset()
{
    _scene->resetZoom();
}
