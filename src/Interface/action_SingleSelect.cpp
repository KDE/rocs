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
#include "Graph.h"
#include "Node.h"
#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_MultiEdge.h"
#include "graphicsitem_OrientedEdge.h"

#include <KLocale>

#include <KDebug>

SingleSelectAction::SingleSelectAction(GraphScene *scene, QObject *parent) 
: AbstractAction(scene, parent){
  setText(i18n ( "Single Select" ));
  setToolTip ( i18n ( "Select Items by clicking on them." ) );
  setIcon ( KIcon ( "single-select" ) );
  _scene = scene;
}

SingleSelectAction::~SingleSelectAction(){
}

void SingleSelectAction::executeRelease(QPointF pos){
  if ( !_graph ) return; 

  QList<QGraphicsItem*> currentSelection = _scene->selectedItems();
  foreach(QGraphicsItem *i, currentSelection){
    i->setSelected(false);
    i->update();
  }


  QGraphicsItem * item = _graphScene->itemAt(pos);
  //! release all current selected items from it's imprisioner.
  if ( ! item ){ 
    emit ItemSelectedChanged(0);
    return; 
  }

  item->setSelected(true);
  item->update();
  
// TODO: Rewrite this to a more ellegant way.
  QObject *obj = 0;
  if (qgraphicsitem_cast<NodeItem*>(item)){
    obj = (qgraphicsitem_cast<NodeItem*>(item)) -> node();
  }
  else if (qgraphicsitem_cast<EdgeItem*>(item)){
    obj = (qgraphicsitem_cast<EdgeItem*>(item)) -> edge();
  }
  else if (qgraphicsitem_cast<OrientedEdgeItem*>(item)){
    obj = (qgraphicsitem_cast<OrientedEdgeItem*>(item)) -> edge();
  }
  else if (qgraphicsitem_cast<MultiEdgeItem*>(item)){
    obj = (qgraphicsitem_cast<MultiEdgeItem*>(item)) -> edge();
  }
  emit ItemSelectedChanged(obj);
}

#include "action_SingleSelect.moc"