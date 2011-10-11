/*  
    This file is part of Rocs.
    Copyright 2008  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#ifndef ACTION_ABSTRACTACTION_H
#define ACTION_ABSTRACTACTION_H

#include <KToggleAction>
#include <QObject>
#include <QString>

class QGraphicsSceneWheelEvent;
class GraphScene;

/*!
\brief the base class for custom actions.
This class provides the basic functionality for all custom actions Used on the pallete bar
that will interact directly with the scene.
*/

// TODO: Changer the behavior of this class, so it sends itself instead of the type  of action to the graphscene.
class AbstractAction : public KToggleAction
{
    Q_OBJECT
public:

    /*! Default Constructor
      \p scene the GraphicsScene that this action will interact.
      \p parent the MainWindow
    */
    AbstractAction(GraphScene *scene, QObject *parent);

    const QString& name() const;
public slots:
    /*! Virtual,  executed when a mouse press occours on the scene.
    \p pos the position on screen that the execution will take part.
    */
    virtual bool executePress(QPointF pos);

    /*! Virtual, executed when a mouse move on the scene.
     \p pos the position on screen that the mouse is now.
    */
    virtual bool executeMove(QPointF pos);

    /*! virtual, executed when a mouse click is released on the scene.
    \p pos the position where the release occourred.
    */
    virtual bool executeRelease(QPointF pos);

     /*! virtual, executed when a mouse click is released on the scene.
    \p pos the position where the release occourred.
    */
     
    virtual bool executeDoubleClick(QPointF pos);
    /*! virtual, executed when a keyboard key is pressed on the scene.
    \p keyEvent the key event from the scene.
    */
    virtual bool executeKeyPress(QKeyEvent *keyEvent);

    /*! virtual, executed when a keyboard key is released on the scene
      \p keyEvent the key event from the scene.
      */
    virtual bool executeKeyRelease(QKeyEvent *keyEvent);

    
    virtual bool executeWheelEvent(QGraphicsSceneWheelEvent *wEvent);
    /*! sends this action to be the executed thing with mouse interaction on the GraphicsView */
    void sendExecuteBit();
    
    bool eventFilter ( QObject * watched, QEvent * event );
protected:

    /*! pointer to the GraphicsScene with the drawing */
    GraphScene *_graphScene;

    QString _name;
};

#endif
