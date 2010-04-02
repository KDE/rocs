/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>

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
#ifndef ACTION_ABSTRACTACTION_H
#define ACTION_ABSTRACTACTION_H

#include <KToggleAction>
#include <QObject>
#include <QString>
class GraphScene;
class Graph;
class GraphDocument;

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

    /*! Atualize the Graph Document that this action will work on
      \p graphDocument the pointer to the active  GraphDocument
    */
    virtual void setActiveGraphDocument( GraphDocument *graphDocument);


    const QString& name() const;
public slots:
    /*! Atualize the Active Graph that this action will work on
      \p graph the pointer to the active  Graph
    */
    virtual void setActiveGraph( Graph *graph);

    /*! Virtual,  executed when a mouse press occours on the scene.
    \p pos the position on screen that the execution will take part.
    */
    virtual void executePress(QPointF pos);

    /*! Virtual, executed when a mouse move on the scene.
     \p pos the position on screen that the mouse is now.
    */
    virtual void executeMove(QPointF pos);

    /*! virtual, executed when a mous click is released on the scene.
    \p pos the position where the release occourred.
    */
    virtual void executeRelease(QPointF pos);

    /*! virtual, executed when a keyboard key is pressed on the scene.
    \p keyEvent the key event from the scene.
    */
    virtual void executeKeyPress(QKeyEvent *keyEvent);

    /*! virtual, executed when a keyboard key is released on the scene
      \p keyEvent the key event from the scene.
      */
    virtual void executeKeyRelease(QKeyEvent *keyEvent);


    /*! sends this action to be the executed thing with mouse interaction on the GraphicsView */
    void sendExecuteBit();

protected:
    /*! pointer to the active graph document */
    GraphDocument *_graphDocument;

    /*! pointer to the active graph */
    Graph *_graph;

    /*! pointer to the GraphicsScene with the drawing */
    GraphScene *_graphScene;

    QString _name;
};

#endif
