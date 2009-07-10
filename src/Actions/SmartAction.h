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


#ifndef ACTION_SMARTACTION_H
#define ACTION_SMARTACTION_H


#include "AbstractAction.h"
#include <QObject>
class AddEdgeAction;
class AddNodeAction;
class MoveNodeAction;
class SelectAction;
class KAction;

class SmartAction : public AbstractAction {
public:
    explicit SmartAction( GraphScene *scene, QObject *parent = 0);
    void setAddEdgeAction(KAction *e);
    void setAddNodeAction(KAction *e);
    void setMoveAction(KAction *e);
    void setSingleSelectAction(KAction *e);

public slots:
    virtual void executePress(QPointF pos);
    virtual void executeMove(QPointF pos);
    virtual void executeRelease(QPointF pos);
    virtual void executeKeyPress(QKeyEvent *keyEvent);
    virtual void executeKeyRelease(QKeyEvent *keyEvent);

private:
    AddNodeAction *_addNodeAction;
    AddEdgeAction *_addEdgeAction;
    SelectAction *_SelectAction;
    MoveNodeAction *_moveAction;
};

#endif
