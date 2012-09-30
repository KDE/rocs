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

#ifndef ADDCONNECTIONHANDACTION_H
#define ADDCONNECTIONHANDACTION_H

#include "VisualEditorExport.h"
#include "AbstractAction.h"
#include <QObject>
#include <QPointF>
#include "CoreTypes.h"

class QGraphicsLineItem;
class DataItem;
class Data;
class DataStructure;
/*!
  \brief The 'Add Connection' Action
*/

class VISUALEDITOR_EXPORT AddConnectionHandAction : public AbstractAction
{
    Q_OBJECT
public:
    /**
     * Default constructor
     * Creates add-edge action that adds data elements of default \see PointerType 0.
     * \param scene is the graph scene at which pointers shall be added
     * \param parent the Parent QOBject that will hold this action.
     */
    explicit AddConnectionHandAction(GraphScene *scene, QObject *parent = 0);

    /**
     * Default constructor
     * Creates add-node action that adds nodes of default \see DataType 0.
     * \param scene is the graph scene at which pointers shall be added
     * \param dataType the type of that new connections shall be created
     * \param parent the parent QObject that will hold this action
    */
    explicit AddConnectionHandAction(GraphScene *scene, PointerTypePtr dataType, QObject *parent = 0);

    ~AddConnectionHandAction();

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

private:
    /*! pointer to the node that suffered the mouse-click */
    DataItem *_from;

    /*! pointer to the node that suffered the mouse-release-click */
    DataItem *_to;

    /*! a temporary line that will connect the two Nodes. */
    QGraphicsLineItem *_tmpLine;

    /*! the point of the first click while adding a new edge */
    QPointF _startPos;

    /*! this boolean is true when we are actually adding a new edge,
    false when we are not. */
    bool _working;

signals:
    void addConnection(Data *from, Data *to);

private:
    PointerTypePtr _pointerType;
};

#endif // ADDCONNECTIONHANDACTION_H
