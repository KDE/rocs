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
#include <boost/shared_ptr.hpp>
#include "Rocs_Typedefs.h"

class QObject;
class QKeyEvent;
class QGraphicsRectItem;
class QGraphicsItem;
class DataItem;
class Data;

/*!
  \brief The 'Move Node' Action
  This action holds the 'Move Node' icon and tooltips, and when it's executed,
  it will move the selected node around the screen.
*/

class SelectMoveHandAction : public AbstractAction
{
    Q_OBJECT
public:
    /*!
      Default constructor
      \param parent the Parent QOBject that will hold this action. ( remove it in the future, maybe? )
    */

    explicit SelectMoveHandAction(GraphScene *scene, QObject *parent = 0);

    /*!
      Default Destructor
    */
    ~SelectMoveHandAction();
public slots:
    /*!
      will be executed when the mouse press a button.
      \param pos the position onscreen of the click.
    */
    bool executePress(QPointF pos);
    /*! will be executed when the mouse moves.
      \param pos the current position of the cursor.
    */
    bool executeMove(QPointF pos);
    /*! will be executed when the mouse releases a click
    \param pos the position of the cursor.
    */
    bool executeRelease(QPointF pos);
    /*! will be executed when the mouse moves.
      \param pos the current position of the cursor.
    */
    bool executeArrowKeyMove(QKeyEvent* keyEvent);


protected:
    /*! the node that will be moved on screen */
    DataItem *_movableNode;

    /*! the model of the NodeItem,
    needs it to modify it's internal value when the move is finished */
    DataPtr _data;
    QPointF _delta;
    QMap<DataItem*, QPointF> _deltas;

signals:
    /*! signal sended when the selection changes. */
    void ItemSelectedChanged(QGraphicsItem *o);

private:
    /*! onscreen temporary rectangle for the selected items */
    QGraphicsRectItem *_selectionRect;
};

#endif
