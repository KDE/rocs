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
#include "UI_GraphScene.h"
#include "Graph.h"
#include "Node.h"
#include "graphicsitem_Node.h"
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

  NodeItem* _node = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));


  //! release all current selected items from it's imprisioner.
  QList<QGraphicsItem*> currentSelection = _scene->selectedItems();
  foreach(QGraphicsItem *i, currentSelection){
    i->setSelected(false);
    i->update();
  }

  if ( ! _node ){ 
    return; 
  }

  _node->setSelected(true);
  _node->update();
  
  emit ItemSelectedChanged(_node->node());

  kDebug() << "Node selected";
}

#include "action_SingleSelect.moc"