/* This file is part of Rocs, a KDE EDU project
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>
   Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include "MoveData.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "Data.h"
#include "Document.h"
#include "DataItem.h"
#include <KLocale>
#include <QGraphicsView>

#include <KDebug>
//#include "settings.h"

MoveDataAction::MoveDataAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Move Node" ));
    setToolTip ( i18n ( "Moves a node around the drawing area." ) );
    setIcon ( KIcon ( "pointer" ) );
    _movableNode = 0;
    _name = "move";
}

MoveDataAction::~MoveDataAction() {
    kDebug() << "Destroyed";
}

bool MoveDataAction::executePress(QPointF pos) {
    if ( !DocumentManager::self()->activeDocument()->activeDataStructure()) {
        return false;
    }

    if (! ( _movableNode = qgraphicsitem_cast<DataItem*>(_graphScene->itemAt(pos)) )) 
        return false;
    
    _deltas.clear();
    if (! _graphScene->selectedItems().contains(_movableNode) ){
        _data = _movableNode->data();
        _delta = pos - QPointF(_data->x(), _data->y());
    }else foreach (QGraphicsItem *item, _graphScene->selectedItems()){

        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)){
            QPointF delta = pos - QPointF(dItem->data()->x(), dItem->data()->y());
            _deltas.insert(dItem, delta);
        }
    }
    return true;
}

bool MoveDataAction::executeMove(QPointF pos) {
    if ( ! _movableNode ) {
        return false;
    }
    
    if (!DocumentManager::self()->activeDocument()->isPointAtDocument(pos)) {
        Document *d = DocumentManager::self()->activeDocument();
        if (pos.x() < d->xLeft()) {
            pos.setX(d->xLeft());
        }
        if (pos.x() > d->xRight()) {
            pos.setX(d->xRight());
        }
        if (pos.y() < d->yTop()) {
            pos.setY(d->yTop());
        }
        if (pos.y() > d->yBottom()) {
            pos.setY(d->yBottom());
        }
    }
    
    if (! _graphScene->selectedItems().contains(_movableNode) ){
        _data -> setPos(pos.x() - _delta.x(), pos.y() - _delta.y());
    }else foreach (QGraphicsItem *item, _graphScene->selectedItems()){
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)){
             dItem->data() -> setPos(pos.x() - _deltas.value(dItem).x(), 
                                     pos.y() - _deltas.value(dItem).y());
        }
    }
    return true;
}

bool MoveDataAction::executeRelease(QPointF pos) {
    if ( !_movableNode ) {
        return false;
    }
    
    if (! _graphScene->selectedItems().contains(_movableNode) ){
        _data -> setPos(pos.x() - _delta.x(), pos.y() - _delta.y());
    }else foreach (QGraphicsItem *item, _graphScene->selectedItems()){
        if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)){
             dItem->data() -> setPos(pos.x() - _deltas.value(dItem).x(), 
                                     pos.y() - _deltas.value(dItem).y());
        }
    }
    
    _movableNode = 0;
    return true;
}
