
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
#ifndef POINTERACTION_H
#define POINTERACTION_H

#include "action_AbstractAction.h"
#include <QObject>

/*! 
  \brief The 'Pointer' Action 
  This action holds the 'Cursor' icon and tooltips, and when it's executed, 
  it will do nothing.
*/

class PointerAction : public AbstractAction{
  public:
    /*! 
      Default constructor 
      \param type the Type for 'Cursor' action. must be unique.
      \param parent the Parent QOBject that will hold this action. ( remove it in the future, maybe? )
    */
  
    PointerAction(GraphScene *scene, QObject *parent = 0);
  
    /*! 
      Default Destructor 
    */
    ~PointerAction();
};

#endif
