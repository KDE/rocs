
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
#include "action_MoveNode.h"
#include "SUI_GraphScene.h"
#include "graph.h"
#include "node.h"
#include "graphicsitem_Node.h"
#include <KLocale>
#include <QGraphicsView>

#include <KDebug>
#include "settings.h"

MoveNodeAction::MoveNodeAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Move Node" ));
    setToolTip ( i18n ( "Moves a node around the drawing area." ) );
    setIcon ( KIcon ( "move-node" ) );
    _movableNode = 0;
}

MoveNodeAction::~MoveNodeAction() {
    kDebug() << "Destroyed";
}

void MoveNodeAction::executePress(QPointF pos) {
    if (_graph == 0) {
        return;
    }
    _movableNode = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));
    if (!_movableNode ) return;
    
    _node = _movableNode->node();
    _graph = qobject_cast<Graph*>(_node->parent());
    _view->setRenderHint(QPainter::Antialiasing, false);
}

void MoveNodeAction::executeMove(QPointF pos) {
    if ( ! _movableNode ) {
        return;
    }
    _movableNode -> updatePos( pos );
}

void MoveNodeAction::executeRelease(QPointF pos) {
    if ( !_movableNode ) {
        return;
    }
    
    _view->setRenderHint(QPainter::Antialiasing, true);
     _movableNode -> updatePos( pos );
     _movableNode = 0;
}

void MoveNodeAction::setView(QGraphicsView *v){
  _view = v;
}