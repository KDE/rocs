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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/
#include "DeleteAction.h"

#include <KLocale>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "GraphScene.h"
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"
#include "DataItem.h"
#include "PointerItem.h"

DeleteAction::DeleteAction(GraphScene* scene, QObject* parent): AbstractAction(scene, parent) {
    setText(i18n ( "Delete" ));
    setToolTip ( i18n ( "Delete items by clicking on them." ) );
    setIcon ( KIcon ( "rocsdelete" ) );
    _name = "delete";
}

void DeleteAction::executePress(QPointF pos)
{
    QGraphicsItem * item = _graphScene->itemAt(pos);
    if ( DataItem *n  = qgraphicsitem_cast<DataItem*>(item) ) {
        n->data()->remove();
    }
    else if ( OrientedEdgeItem *e = qgraphicsitem_cast<OrientedEdgeItem*>(item) ) {
        e->pointer()->remove();
    }
}

void DeleteAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Delete) {
        
    }
}

