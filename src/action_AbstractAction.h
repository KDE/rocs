
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
#ifndef ABSTRACTACTION_H
#define ABSTRACTACTION_H

#include <KAction>
#include <QObject>

#include "UI_GraphScene.h"

/*! 
\brief the base class for custom actions.
This class provides the basic functionality for all custom actions Used on the pallete bar. 
*/

class AbstractAction : public KAction
{
Q_OBJECT
public: 
	/*! 
		Default constructor 
		\param type the Type of the Action, each action must have a diferent one. doesn't matter the number.
		\param parent the pointer of the Container that will hold the action. (hm... should remove this in the future? )
	*/
	AbstractAction(int type, QObject *parent);

	/*! 
		the Type of the action 
		\return return the type of the action.
	*/
	int type() { return _type; }

	/*! 
		when the user selects a different GraphFile from the FileArea this method will be invocked
		\param graphScene the new GraphScene where this action will work on.

	 */
	virtual void changeGraphScene(GraphScene *graphScene);
		
public slots:
	
	/*! 
		executed just after the user clicks on the Graph Screen 
		\param type the type of the action to be executed. if the type is the same as this action, then this action will be executed.
		\param pos the position on the QGraphcsView area that will be affected when this action is executed.
	
	*/
	virtual void execute(int type, QPointF pos) = 0;
	
	/*! Changes the action that will be executed 
	after the user clicks on the Graph Screen */ 
	void sendExecuteBit();

protected:

	int _type; //! the type, so the program can know what is being executed.
	GraphScene *_graphScene; //! The main drawing area.


};

#endif