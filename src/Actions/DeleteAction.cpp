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
#include "node.h"
#include "edge.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"

DeleteAction::DeleteAction(GraphScene* scene, QObject* parent): AbstractAction(scene, parent) {
    setText(i18n ( "Delete" ));
    setToolTip ( i18n ( "Delete items by clicking on them." ) );
    setIcon ( KIcon ( "rocsdelete" ) );
    _name = "delete";
}

void DeleteAction::executePress(QPointF pos)
{
    QGraphicsItem * item = _graphScene->itemAt(pos);
    if ( NodeItem *n  = qgraphicsitem_cast<NodeItem*>(item) ) {
        qDebug() << "Trying to remove node. NodeItem: " << n->objectName() ;
        n->node()->remove();
    }
    else if ( OrientedEdgeItem *e = qgraphicsitem_cast<OrientedEdgeItem*>(item) ) {
        qDebug() << "Should have deleted the oriented node.";
        e->edge()->remove();
    }
    qDebug() << "Item Removed!";
}

void DeleteAction::executeKeyRelease(QKeyEvent* keyEvent)
{
    if (keyEvent->key() == Qt::Key_Delete) {
        qDebug() << "you pressed the Delete key... big deal.";
    }
}

