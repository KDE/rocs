
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

void SelectAction::executePress(QPointF pos) {
    if (! DocumentManager::self()->activeDocument()->activeDataStructure() ){
      return;
    }
    _graphScene->views().at(0)->setInteractive(true);
    _graphScene->views().at(0)->setDragMode(QGraphicsView::RubberBandDrag);
    
    _p1 = pos;
}

void SelectAction::executeRelease(QPointF pos) {
    if (! DocumentManager::self()->activeDocument()->activeDataStructure() ){
      return;
    }

    QList<QGraphicsItem*> currentSelection = _graphScene->selectedItems();
    foreach(QGraphicsItem *i, currentSelection) {
        i->setSelected(false);
        i->update();
    }

    if (pos == _p1) {
        singleSelect(pos);
    }
    else {
        multiSelect(pos);
    }
}

void SelectAction::multiSelect(QPointF pos) {
    QList<QGraphicsItem*> items = _graphScene->items(QRectF(_p1, pos));
    foreach(QGraphicsItem *i, items) {
        if (!( qgraphicsitem_cast<DataItem*>(i) || qgraphicsitem_cast<PointerItem*>(i))) {
            items.removeAll(i);
        }
    }
    if (items.size() == 1) {
        singleSelect(items[0]->pos());
        return;
    }

    foreach(QGraphicsItem *i, items) {
        i->setSelected(true);
        i->update();
    }
}

#include <QtAlgorithms>
bool zValue(QGraphicsItem *i1, QGraphicsItem *i2) {
    return i1->zValue() < i2->zValue();
}

void SelectAction::singleSelect(QPointF pos) {
    QGraphicsItem * item = 0;
    QGraphicsEllipseItem *tmpItem = new QGraphicsEllipseItem(pos.x()-5,pos.y()-5,10,10);
    _graphScene->addItem(tmpItem);

    if (tmpItem->collidingItems().empty()) {
        kDebug() << "No Collisions";
        _graphScene->removeItem(tmpItem);
        delete tmpItem;
        return;
    } else {
        QList<QGraphicsItem*> items = tmpItem->collidingItems();

        qSort(items.begin(), items.end(), zValue);
        item = items.at(items.size()-1);
        if (item->zValue() == -1000) {
            emit ItemSelectedChanged(0);
            delete tmpItem;
            return;
        }
    }

    item->setSelected(true);
    emit ItemSelectedChanged(item);
    item->update();
    qDebug() << "Item Selected!";
    delete tmpItem;
}
#include "Select.moc"
