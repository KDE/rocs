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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
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
    void executePress(QPointF pos);

    /*! when somebody press the delete key, the system will try to delete all selected nodes.
    \p keyEvent the key to be processed.
    */
    void executeKeyRelease(QKeyEvent* keyEvent);
};

#endif // DELETEACTION_H
