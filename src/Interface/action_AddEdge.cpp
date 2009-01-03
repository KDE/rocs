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

#include "action_AddEdge.h"
#include "SUI_GraphScene.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "graphicsitem_Node.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_OrientedEdge.h"
#include <KLocale>

#include <KDebug>

AddEdgeAction::AddEdgeAction(GraphScene *scene, QObject *parent) 
: AbstractAction(scene, parent){
  setText(i18n ( "Add Edge" ));
  setToolTip ( i18n ( "Creates a new edge between 2 nodes" ) );
  setIcon ( KIcon ( "add-edge" ) );

  _nodeFrom = 0;
  _nodeTo   = 0;
  _tmpLine  = 0;
  _working = false;
}

AddEdgeAction::~AddEdgeAction(){
  kDebug() << "Destroyed";
}

void AddEdgeAction::executePress(QPointF pos){
  if (_working) return;
  if ( ! _graph ) return; 

  _working = true;
  _nodeFrom = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));

  if ( ! _nodeFrom ) return;
  _startPos = QPointF(_nodeFrom->node()->property("x").toInt(), 
		      _nodeFrom->node()->property("y").toInt()); 
}

void AddEdgeAction::executeMove(QPointF pos){
  if ( !_graph ) return; 
  if ( !_nodeFrom ) return; 
  
  if ( !_tmpLine ){
    _tmpLine = new QGraphicsLineItem( _startPos.x(), _startPos.y(), pos.x(), pos.y());
    _graphScene->addItem(_tmpLine);  
  }
  else{
    _tmpLine->setLine(_startPos.x(), _startPos.y(), pos.x(), pos.y());
  }
}

void AddEdgeAction::executeRelease(QPointF pos){
  if ( !_working ) return;
  if ( !_graph ) return; 

  if( _tmpLine ){ 
    delete _tmpLine; 
    _tmpLine = 0;
  }

  _nodeTo = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));
  if ( ! _nodeTo ){ 
    _nodeFrom = 0;
    _working = false;
    return; 
  }

  Edge *e = _graph -> addEdge( _nodeFrom->node(),  _nodeTo->node() );
  if (e == 0){
    _nodeFrom = 0;
    _working = false;
     return;
  }
  
  QGraphicsItem *edgeItem = 0;
  if ( !_graph->directed() ){ 
    edgeItem = new EdgeItem(e); 
  }
  else{ 
    edgeItem = new OrientedEdgeItem(e);
  }

  _graphScene->addItem(edgeItem);

  _nodeFrom = 0;
  _nodeTo = 0;
  _working = false;
}
