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

#ifndef DELETEACTION_H
#define DELETEACTION_H

#include "AbstractAction.h"

/*! this class defines the delete button on the pallete. */
class DeleteAction : public AbstractAction
{
public:
    /*! default constructor
    \p scene the graph scene
    \p parent the mainwindow
    */
    DeleteAction(GraphScene* scene, QObject* parent);

    /*! start the node deletion algorithm on the specified point.
    \p pos the position of the click.
    */
    bool executePress(QPointF pos);

    /*! when somebody press the delete key, the system will try to delete all selected nodes.
    \p keyEvent the key to be processed.
    */
    bool executeKeyRelease(QKeyEvent* keyEvent);
};

#endif // DELETEACTION_H
