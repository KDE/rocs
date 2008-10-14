
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

#ifndef UI_GRAPHSCENE_H
#define UI_GRAPHSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>

class MainWindow;
class QGraphicsSceneMouseEvent;
class AbstractAction;

namespace libgraph{
  class GraphDocument;
}



/*! 
\brief The graph drawing Area 

This class holds all the nodes, edges and stuff that will appear on the screen.
it also needs to respond to all actions created on the pallete bar 
and invockable from the Script Side.

*/

class GraphScene : public QGraphicsScene{
Q_OBJECT
public:
    /*! 
    default constructor 
    \param parent the MainObject that holds this container.
    \param item all the Graphs of the file.
    */

    GraphScene ( QObject* parent );
    void setAction(AbstractAction *action);
    void setSceneRect(qreal width, qreal height);

private:
   /*! 
    this will happen when the user clicks on the screen 
    \param mouseEvent the MouseEvent send by Qt
    */
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    
    /*! 
    this will happen when the user clicks on the screen 
    \param mouseEvent the MouseEvent send by Qt
    */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    
    /*! 
    this will happen when the user clicks on the screen 
    \param mouseEvent the MouseEvent send by Qt
    */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);


  AbstractAction *_action;

};
#endif
