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

#ifndef ACTION_ADDDATAHANDACTION_H
#define ACTION_ADDDATAHANDACTION_H

#include "AbstractAction.h"
#include <QObject>
#include <Rocs_Typedefs.h>

/*!
  \brief The 'Add Data' Action
  This action holds the 'Add Data' icon and tooltips, and when it's executed,
  it will place a new data element on the QGraphicsView that holds all the items.
*/

class AddDataHandAction : public AbstractAction
{
    Q_OBJECT
public:
    /**
     * Default constructor
     * Creates add-node action that adds data elements of default \see DataType 0.
     * \param scene is the graph scene at which data elements shall be added
     * \param parent the Parent QObject that will hold this action.
     */
    explicit AddDataHandAction(GraphScene *scene, QObject *parent = 0);

    /**
     * Default constructor
     * Creates add-node action that adds nodes of default \see DataType 0.
     * \param scene is the graph scene at which data elements shall be added
     * \param dataType the type of that new data elements shall be created
     * \param parent the Parent QOBject that will hold this action.
    */
    explicit AddDataHandAction(GraphScene *scene, DataTypePtr dataType, QObject *parent = 0);

    ~AddDataHandAction();

    /**
     * Set \see DataType of which data elements are created.
     * \param dataType is the dataType associated to the action
     */
    void setDataType(DataTypePtr dataType);

public slots:
    /**
     * This method creates data element for previously specified type at position \p pos.
     * \param pos the position on the screen that the new data element will be placed
     * \return true if the data element could be added, otherwise false
     */
    bool executePress(QPointF pos);

signals:
    void addNode(QString name, QPointF pos);

private:
    DataTypePtr _dataType;
};

#endif
