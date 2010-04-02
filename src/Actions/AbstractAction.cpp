
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
#include "AbstractAction.h"
#include "GraphScene.h"
#include "graph.h"
#include "graphDocument.h"
#include <QPointF>
#include <KDebug>

AbstractAction::AbstractAction(GraphScene *scene, QObject *parent)
        : KToggleAction(parent) {
    _graphDocument = 0;
    _graph = 0;
    _graphScene = scene;

    connect(this, SIGNAL(triggered()),
            this, SLOT( sendExecuteBit() ));
}


void AbstractAction::setActiveGraphDocument(  GraphDocument *graphDocument) {
    _graphDocument = graphDocument;
}

void AbstractAction::setActiveGraph( Graph *graph) {
    _graph = graph;
}

void AbstractAction::sendExecuteBit() {
    _graphScene -> setAction ( this );
}

const QString& AbstractAction::name() const {
    return _name;
}
void AbstractAction::executePress(QPointF) {}
void AbstractAction::executeMove(QPointF) {}
void AbstractAction::executeRelease(QPointF) {}
void AbstractAction::executeKeyPress(QKeyEvent*) {}
void AbstractAction::executeKeyRelease(QKeyEvent*) {}

