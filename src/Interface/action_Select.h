
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
#ifndef ACTION_SELECT_H
#define ACTION_SELECT_H

#include "action_AbstractAction.h"
#include <QObject>
#include <QPointF>

class NodeItem;
class Node;
class QGraphicsRectItem;
class QGraphicsItem;

/*!
  \brief The 'Add Node' Action
  This action holds the 'Add Node' icon and tooltips, and when it's executed,
  it will place a new node on the QGraphicsView that holds all the items.
*/

class SelectAction : public AbstractAction
{
    Q_OBJECT
public:
    /*!
      Default constructor
      \param parent the Parent QOBject that will hold this action. ( remove it in the future, maybe? )
    */

    explicit SelectAction(GraphScene *scene, QObject *parent = 0);

    /*!
      Default Destructor
    */
    ~SelectAction();
public slots:
    /*!
      will be executed when the mouse press a button.
      \param pos the position onscreen of the click.
    */
    void executePress(QPointF pos);
    /*! will be executed when the mouse moves.
      \param pos the current position of the cursor.
    */
    void executeMove(QPointF pos);

    /*! will be executed when the mouse releases a click
    \param pos the position of the cursor.
    */
    void executeRelease(QPointF pos);

signals:
    void ItemSelectedChanged(QGraphicsItem *o);
    
private:
    void singleSelect(QPointF pos);
    void multiSelect(QPointF pos);
     QPointF _p1;
     QPointF _p2;
     QGraphicsRectItem *_selectionRect;

};

#endif
