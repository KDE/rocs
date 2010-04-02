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

#include "AddEdge.h"
#include "GraphScene.h"
#include "graph.h"
#include "node.h"
#include "edge.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"
#include <KLocale>

#include <KDebug>

AddEdgeAction::AddEdgeAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Add Edge" ));
    setToolTip ( i18n ( "Creates a new edge between 2 nodes" ) );
    setIcon ( KIcon ( "rocsaddedge" ) );

    _nodeFrom = 0;
    _nodeTo   = 0;
    _tmpLine  = 0;
    _working = false;
    _name = "add-edge";
}

AddEdgeAction::~AddEdgeAction() {
    kDebug() << "Destroyed";
}

void AddEdgeAction::executePress(QPointF pos) {
    if (_working) return;
    if ( ! _graph ) return;
    if (_graph->readOnly()) return;
    _working = true;
    _nodeFrom = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));

    if ( ! _nodeFrom ) {
        _working = false;
        return;
    }
    _startPos = QPointF(_nodeFrom->node()->x(), _nodeFrom->node()->y());
}

void AddEdgeAction::executeMove(QPointF pos) {
    if ( !_graph ) return;
    if ( !_nodeFrom ) return;

    if ( !_tmpLine ) {
        _tmpLine = new QGraphicsLineItem( _startPos.x(), _startPos.y(), pos.x(), pos.y());
        _graphScene->addItem(_tmpLine);
    }
    else {
        _tmpLine->setLine(_startPos.x(), _startPos.y(), pos.x(), pos.y());
    }
}

void AddEdgeAction::executeRelease(QPointF pos) {
    if ( !_working ) return;
    if ( !_graph ) return;

    if ( _tmpLine ) {
        delete _tmpLine;
        _tmpLine = 0;
    }

    _nodeTo = qgraphicsitem_cast<NodeItem*>(_graphScene->itemAt(pos));
    if (  _nodeTo ) {
        emit addEdge( _nodeFrom->node(),  _nodeTo->node() );
    }

    _nodeFrom = 0;
    _nodeTo = 0;
    _working = false;
}

void AddEdgeAction::setActiveGraph(Graph* graph){
    if (_graph) disconnect(this, 0, _graph, 0);
    _graph = graph;
    connect(this, SIGNAL(addEdge(Node*,Node*)), _graph, SLOT(addEdge(Node*,Node*)));
}