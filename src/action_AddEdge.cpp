
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
#include "UI_GraphScene.h"
#include "libgraph_Graph.h"

#include <KLocale>

#include <KDebug>

AddEdgeAction::AddEdgeAction(GraphScene *scene, QObject *parent) 
: AbstractAction(scene, parent){
  setText(i18n ( "Add Edge" ));
  setToolTip ( i18n ( "Creates a new edge between 2 nodes" ) );
  setIcon ( KIcon ( "pointer" ) );
  setCheckable ( true );
  setChecked ( false );

  connect(this, SIGNAL(triggered()), this, SLOT( sendExecuteBit() ));
}

AddEdgeAction::~AddEdgeAction(){
  kDebug() << "Destroyed";
}

void AddEdgeAction::executePress(QPointF pos){
  if (_graph == 0){
    kDebug() << "Error, Graph == 0";
    return;
  }
  
  kDebug() << " NOT IMPLEMENTED YET ";

}

void AddEdgeAction::executeMove(QPointF pos){}

void AddEdgeAction::executeRelease(QPointF pos){}