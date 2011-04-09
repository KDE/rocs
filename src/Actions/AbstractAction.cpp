
/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "AbstractAction.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "Document.h"
#include <QPointF>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <KDebug>

AbstractAction::AbstractAction(GraphScene *scene, QObject *parent)
        : KToggleAction(parent) {
    _graphScene = scene;

    connect(this, SIGNAL(triggered()),
            this, SLOT( sendExecuteBit() ));
}

void AbstractAction::sendExecuteBit() {
    _graphScene -> setAction ( this );
}

const QString& AbstractAction::name() const {
    return _name;
}

bool AbstractAction::eventFilter ( QObject * watched, QEvent * event ){
    Q_UNUSED(watched);
    switch(event->type()){
        case QEvent::GraphicsSceneMousePress : {
            QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
            if (e->button() == Qt::LeftButton){
                return executePress(e->scenePos());
            }
        }
        case QEvent::GraphicsSceneMouseMove  :{
            QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
            return executeMove(e->scenePos());
        }
        case QEvent::GraphicsSceneMouseRelease :{
             QGraphicsSceneMouseEvent *e = static_cast<QGraphicsSceneMouseEvent*>(event);
            return executeRelease(e->scenePos());
        }
        case QEvent::GraphicsSceneMouseDoubleClick :{
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
        default:  return false;
    }
    return false;
}

bool AbstractAction::executePress(QPointF pos){   Q_UNUSED(pos); return false; }
bool AbstractAction::executeMove(QPointF pos){    Q_UNUSED(pos); return false; }
bool AbstractAction::executeRelease(QPointF pos) {Q_UNUSED(pos); return false; }
bool AbstractAction::executeDoubleClick(QPointF pos){ Q_UNUSED(pos); return false; }
bool AbstractAction::executeKeyPress(QKeyEvent *keyEvent){ Q_UNUSED(keyEvent);   return false; }
bool AbstractAction::executeKeyRelease(QKeyEvent *keyEvent){ Q_UNUSED(keyEvent); return false; }