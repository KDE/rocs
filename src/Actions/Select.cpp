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
#include <QKeyEvent>

SelectAction::SelectAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Select" ));
    setToolTip ( i18n ( "Select Items by clicking on them." ) );
    setIcon ( KIcon ( "rocsselect" ) );
    _name = "select";
    
    connect (_graphScene, SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(executeKeyRelease(QKeyEvent*)));
}

SelectAction::~SelectAction() {}

bool SelectAction::executePress(QPointF pos) {
    Q_UNUSED(pos);
    if (! DocumentManager::self()->activeDocument()->activeDataStructure() ){
      return false;
    }
    _graphScene->views().at(0)->setInteractive(true);
    _graphScene->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    
    return true;
}

bool SelectAction::executeRelease(QPointF pos) {
    Q_UNUSED(pos);
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

bool SelectAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    if (keyEvent->matches(QKeySequence::SelectAll)) {
        foreach (QGraphicsItem *item, _graphScene->items()){
            if (DataItem *dItem = qgraphicsitem_cast<DataItem*>(item)){
                dItem->setSelected(true);
            }
        }
        return true;
    }
    return false;
}


#include "Select.moc"
