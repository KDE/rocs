
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
#include "AddNodeAction.h"
#include <KLocale>
#include "UI_GraphScene.h"
#include <iostream>

AddNodeAction::AddNodeAction(int type, QObject *parent) : AbstractAction(type, parent)
{
	setText(i18n ( "Add Node" ));
	setToolTip ( i18n ( "Creates a new node at the click position on the drawing area." ) );
	setIcon ( KIcon ( "pointer" ) );
	setCheckable ( true );
	setChecked ( false );
	setProperty ( "rocs_action", GraphScene::AddNode );
}

AddNodeAction::~AddNodeAction(){}

void AddNodeAction::execute(int action, QPointF pos)
{
	if (action != _type) return;
	
	/*!inserts a new node on position pos. 
	then adds it to the scene so it can be draw.
	and append it on the nodes list for the scripting interface 
	*/
	
	Node *node = new Node( _graphScene );
	node  ->  setPos( pos );
	_graphScene -> addItem( node );
	_graphScene -> activeGraph() -> nodes.append( node );
	node -> index = _graphScene -> activeGraph() -> nodes.size()-1;
}
