/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "SmartAction.h"
#include "AddEdge.h"
#include "AddNode.h"
#include "MoveNode.h"
#include "Select.h"

#include <KAction>
#include <KLocale>
SmartAction::SmartAction(GraphScene *scene, QObject *parent)
        : AbstractAction(scene, parent) {
    setText(i18n ( "Smart Tool" ));
    setToolTip ( i18n ( "Tries to do what it thinks that's good for the soul." ) );
    setIcon ( KIcon ( "smart-tool" ) );
}

void SmartAction::executePress(QPointF) {

}

void SmartAction::executeMove(QPointF) {

}

void SmartAction::executeRelease(QPointF) {

}

void SmartAction::executeKeyPress(QKeyEvent */*keyEvent*/) {

}

void SmartAction::executeKeyRelease(QKeyEvent */*keyEvent*/) {

}

void SmartAction::setAddEdgeAction(KAction *e) {
    _addEdgeAction = qobject_cast<AddEdgeAction*>(e);
}

void SmartAction::setAddNodeAction(KAction *e) {
    _addNodeAction = qobject_cast<AddNodeAction*>(e);
}

void SmartAction::setMoveAction(KAction *e) {
    _moveAction = qobject_cast<MoveNodeAction*>(e);
}

void SmartAction::setSingleSelectAction(KAction *e) {
    _SelectAction = qobject_cast<SelectAction*>(e);
}
