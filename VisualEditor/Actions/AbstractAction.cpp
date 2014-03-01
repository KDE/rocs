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

#include "AbstractAction.h"
#include "Scene/GraphScene.h"
#include "DataStructure.h"
#include "Document.h"
#include <QPointF>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

AbstractAction::AbstractAction(GraphScene *scene, QObject *parent)
    : KToggleAction(parent)
{
    _graphScene = scene;
}

void AbstractAction::sendExecuteBit()
{
    _graphScene -> setAction(this);
}

const QString& AbstractAction::name() const
{
    return _name;
}

bool AbstractAction::eventFilter(QObject * watched, QEvent * event)
{
    Q_UNUSED(watched);
    switch (event->type()) {
    case QEvent::GraphicsSceneMousePress : {
        QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
        if (e->button() == Qt::LeftButton) {
            return executePress(e->scenePos());
        }
        // a right click shall not trigger any QAction parent class function
        // since this is reserved for the context menu
        // TODO add specific entries to graph scene context menu here
        if (e->button() == Qt::RightButton) {
            e->accept();
            return true;
        }
    }
    case QEvent::GraphicsSceneMouseMove  : {
        QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
        return executeMove(e->scenePos());
    }
    case QEvent::GraphicsSceneMouseRelease : {
        QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
        return executeRelease(e->scenePos());
    }
    case QEvent::GraphicsSceneMouseDoubleClick : {
        QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
        return executeDoubleClick(e->scenePos());
    }
    case QEvent::KeyPress: {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        return executeKeyPress(e);
    }
    case QEvent::KeyRelease: {
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        return executeKeyRelease(e);
    }
    case QEvent::GraphicsSceneWheel: {
        QGraphicsSceneWheelEvent *e = static_cast<QGraphicsSceneWheelEvent*>(event);
        return executeWheelEvent(e);
    }
    default:  return false;
    }
    return false;
}

bool AbstractAction::executePress(QPointF pos)
{
    Q_UNUSED(pos);
    return false;
}
bool AbstractAction::executeMove(QPointF pos)
{
    Q_UNUSED(pos);
    return false;
}
bool AbstractAction::executeRelease(QPointF pos)
{
    Q_UNUSED(pos);
    return false;
}
bool AbstractAction::executeDoubleClick(QPointF pos)
{
    Q_UNUSED(pos);
    return false;
}
bool AbstractAction::executeKeyPress(QKeyEvent *keyEvent)
{
    Q_UNUSED(keyEvent);
    return false;
}
bool AbstractAction::executeKeyRelease(QKeyEvent *keyEvent)
{
    Q_UNUSED(keyEvent);
    return false;
}
bool AbstractAction::executeWheelEvent(QGraphicsSceneWheelEvent* wEvent)
{
    Q_UNUSED(wEvent);
    return false;
}
