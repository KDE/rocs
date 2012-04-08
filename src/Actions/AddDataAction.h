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

#ifndef ACTION_ADDDATAACTION_H
#define ACTION_ADDDATAACTION_H

#include "AbstractAction.h"
#include <QObject>
#include <Rocs_Typedefs.h>

/*!
  \brief The 'Add Node' Action
  This action holds the 'Add Node' icon and tooltips, and when it's executed,
  it will place a new node on the QGraphicsView that holds all the items.
*/

class AddDataAction : public AbstractAction
{
    Q_OBJECT
public:
    /*!
      Default constructor
      \param type the Type for 'Add Node' action. must be unique.
      \param parent the Parent QOBject that will hold this action. ( remove it in the future, maybe? )
    */
    explicit AddDataAction(GraphScene *scene, QObject *parent = 0);

    /*!
      Default Destructor
    */
    ~AddDataAction();

    void setActiveGraph(DataStructurePtr graph);

    void setAddPosition(QPointF position);

public slots:
    /*!
      this will be executed if type is the same as this action's type.
      \param pos the position on the screen that the new node will be placed.

    */
    bool executePress(QPointF pos);

    bool executePress();

signals:
    void addNode(QString name, QPointF pos);

private:
    QPointF _position; // add data at this point
};

#endif
