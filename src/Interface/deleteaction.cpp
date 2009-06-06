/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "deleteaction.h"
#include <KLocale>
#include <QGraphicsItem>
#include <graphicsitem_Node.h>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "SUI_GraphScene.h"
#include "node.h"
#include "edge.h"
#include <graphicsitem_Edge.h>
#include <graphicsitem_OrientedEdge.h>

DeleteAction::DeleteAction(GraphScene* scene, QObject* parent): AbstractAction(scene, parent){
    setText(i18n ( "Delete" ));
    setToolTip ( i18n ( "Delete items by clicking on them.." ) );
    setIcon ( KIcon ( "remove-item" ) );
}

void DeleteAction::executePress(QPointF pos)
{
   QGraphicsItem * item = _graphScene->itemAt(pos);
   if ( NodeItem *n  = qgraphicsitem_cast<NodeItem*>(item) ){ 
	Node *node = n->node();
	node->self_remove();
	_graphScene->updateDocument(); //! TODO: we don't want to redraw the whole document, we just want to remove the edges connected to this node, and this node on the screen;
        return;
    }
    else if( EdgeItem *e = qgraphicsitem_cast<EdgeItem*>(item) ){
	Edge *edge = e->edge();
	edge->self_remove();
	//_graphScene->updateDocument(); //! TODO: we don't want to redraw the whole document, we just want to remove the edges connected to this node, and this node on the screen;
        return;
    }
   else if( OrientedEdgeItem *e = qgraphicsitem_cast<OrientedEdgeItem*>(item) ){
	Edge *edge = e->edge();
	edge->self_remove();
	//_graphScene->updateDocument(); //! TODO: we don't want to redraw the whole document, we just want to remove the edges connected to this node, and this node on the screen;
        return;
    }
  qDebug() << "Item Removed!";
}

void DeleteAction::executeKeyRelease(QKeyEvent* keyEvent)
{
  if (keyEvent->key() == Qt::Key_Delete){
    qDebug() << "you pressed the Delete key... big deal.";
  }
}

