
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

#include "Select.h"
#include "GraphScene.h"
#include "DataStructure.h"
#include "Data.h"
#include "DocumentManager.h"
#include "Document.h"
#include "Pointer.h"
#include "DataItem.h"
#include "PointerItem.h"

#include <KLocale>
#include <KDebug>
#include <QDebug>
#include <QGraphicsView>

SelectAction::SelectAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Select" ));
    setToolTip ( i18n ( "Select Items by clicking on them." ) );
    setIcon ( KIcon ( "rocsselect" ) );
    _name = "select";
}

SelectAction::~SelectAction() {}

bool SelectAction::executePress(QPointF pos) {
    if (! DocumentManager::self()->activeDocument()->activeDataStructure() ){
      return false;
    }
    _graphScene->views().at(0)->setInteractive(true);
    _graphScene->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    
    return true;
}

bool SelectAction::executeRelease(QPointF pos) {
    if (! DocumentManager::self()->activeDocument()->activeDataStructure() ){
      return false;
    }

    QList<QGraphicsItem*> currentSelection = _graphScene->selectedItems();
    foreach(QGraphicsItem *i, currentSelection) {
        if (!( qgraphicsitem_cast<DataItem*>(i) || qgraphicsitem_cast<PointerItem*>(i))) {
            currentSelection.removeOne(i);
        }
    }
    _graphScene->views().at(0)->setDragMode(QGraphicsView::NoDrag);
    
    return true;
    
}

#include "Select.moc"
