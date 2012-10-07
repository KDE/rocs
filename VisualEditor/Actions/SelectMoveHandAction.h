/*
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SELECTMOVEHANDACTION_H
#define SELECTMOVEHANDACTION_H

#include "AbstractAction.h"
#include "CoreTypes.h"

class QObject;
class QKeyEvent;
class QGraphicsRectItem;
class QGraphicsItem;
class DataItem;

/**
 * \class SelectMoveHandAction
 * This action holds the 'Move Node' icon and tooltips, and when it's executed,
 * it will move the selected node around the screen.
 */
class SelectMoveHandAction : public AbstractAction
{
    Q_OBJECT

public:
    /**
     * Default constructor.
     *
     * \param scene is the graphics scene that contains the data elements
     * \param parent the parent QObject that holds this action
     */
    explicit SelectMoveHandAction(GraphScene *scene, QObject *parent = 0);

    /**
     * Default Destructor.
     */
    ~SelectMoveHandAction();

public slots:
    /**
     * Executed when the left mouse button is pressed at the scene.
     *
     * \param pos the onscreen position of the click
     */
    bool executePress(QPointF pos);

    /**
     * Executed when the mouse is moved at the scene scene.
     *
     * \param pos the current onscreen position of the mouse
     */
    bool executeMove(QPointF pos);

    /**
     * Executed when the left mouse button is released
     *
     * \param pos the onscreen position of the mouse
     */
    bool executeRelease(QPointF pos);

    /**
     * Executed when a key is pressed.
     *
     * \param keyEvent the key press event
     */
    bool executeKeyPress(QKeyEvent* keyEvent);

    /**
     * Executed when a key is released.
     *
     * \param keyEvent the key press event
     */
    bool executeKeyRelease(QKeyEvent* keyEvent);

private:
    /**
     * Moves all currently selected items by \p movement.
     *
     * \param movement is the relative movement vector
     */
    void moveSelectedItems(QPointF movement);

    DataItem *_currentItem; // currently active item
    QMap<DataItem*, QPointF> _deltas; // original data item positions from begin of movement
    QGraphicsRectItem *_selectionRect; // on-screen rectangle for selecting items
    bool _selectionMode; // true while CTRL key is pressed
};

#endif
