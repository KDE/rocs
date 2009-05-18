
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

#include "action_SingleSelect.h"
#include "SUI_GraphScene.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_MultiEdge.h"
#include "graphicsitem_OrientedEdge.h"

#include <KLocale>
#include <KDebug>

SingleSelectAction::SingleSelectAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Single Select" ));
    setToolTip ( i18n ( "Select Items by clicking on them." ) );
    setIcon ( KIcon ( "single-select" ) );
    _selectionRect = 0;
}

SingleSelectAction::~SingleSelectAction() {
}

void SingleSelectAction::executePress(QPointF pos){
  _p1 = pos;
  _selectionRect = new QGraphicsRectItem();
  _graphScene->addItem(_selectionRect);
}

void SingleSelectAction::executeMove(QPointF pos){
  if (_selectionRect == 0) return;
  _selectionRect->setRect(QRectF(_p1, pos));
}

void SingleSelectAction::executeRelease(QPointF pos) {
    if ( !_graph ) return;
    if ( _selectionRect == 0) return;
    
    _graphScene->removeItem(_selectionRect);
    
    delete _selectionRect;
    _selectionRect = 0;
    
    QList<QGraphicsItem*> currentSelection = _graphScene->selectedItems();
    kDebug() << "Number of selected Items: " << currentSelection.size();
    foreach(QGraphicsItem *i, currentSelection) {
        i->setSelected(false);
        i->update();
    }
    
    if (pos == _p1){
      singleSelect(pos);
    }
    else{
      multiSelect(pos);
    }
    
}

void SingleSelectAction::multiSelect(QPointF pos){
  QList<QGraphicsItem*> items = _graphScene->items(QRectF(_p1, pos));
  foreach(QGraphicsItem *i, items){
    i->setSelected(true);
    i->update();
  }
}

void SingleSelectAction::singleSelect(QPointF pos){
    QGraphicsItem * item = _graphScene->itemAt(pos);
    if ( ! item ) {
        emit ItemSelectedChanged(0);
        return;
    }

    item->setSelected(true);
    QObject *obj = 0;
    if (qgraphicsitem_cast<NodeItem*>(item)) {
        obj = (qgraphicsitem_cast<NodeItem*>(item)) -> node();
    }
    else if (qgraphicsitem_cast<EdgeItem*>(item)) {
        obj = (qgraphicsitem_cast<EdgeItem*>(item)) -> edge();
    }
    else if (qgraphicsitem_cast<OrientedEdgeItem*>(item)) {
        obj = (qgraphicsitem_cast<OrientedEdgeItem*>(item)) -> edge();
    }
    emit ItemSelectedChanged(obj);
    item->update();
}
#include "action_SingleSelect.moc"
