/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008  Ugo Sangiori <ugorox@gmail.com>

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

#ifndef ACTION_SELECT_H
#define ACTION_SELECT_H

#include "AbstractAction.h"
#include <QObject>
#include <QPointF>

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
    bool executePress(QPointF pos);

    /*! will be executed when the mouse releases a click
    \param pos the position of the cursor.
    */
    bool executeRelease(QPointF pos);

    bool executeKeyRelease(QKeyEvent* keyEvent);

signals:
    /*! signal sended when the selection changes. */
    void ItemSelectedChanged(QGraphicsItem *o);

private:
    /*! onscreen temporary rectangle for the selected items */
    QGraphicsRectItem *_selectionRect;

};

#endif
