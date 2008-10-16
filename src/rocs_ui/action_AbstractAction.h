
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

class GraphScene;

namespace libgraph{
  class Graph;
  class GraphDocument;
}

/*! 
\brief the base class for custom actions.
This class provides the basic functionality for all custom actions Used on the pallete bar. 
*/

// TODO: Changer the behavior of this class, so it sends itself instead of the type  of action to the graphscene.
class AbstractAction : public KAction
{
Q_OBJECT
public: 

  /*! Default Constructor  
    \param scene the GraphicsScene that this action will interact.
    \param parent the MainWindow
  */
  AbstractAction(GraphScene *scene, QObject *parent);

  /*! Atualize the Graph Document that this action will work on 
    \param graphDocument the pointer to the active libgraph::GraphDocument
  */
  void setGraphDocument(libgraph::GraphDocument *graphDocument);

public slots:
  /*! Atualize the Active Graph that this action will work on 
    \param graph the pointer to the active libgraph::Graph
  */
  virtual void setGraph(libgraph::Graph *graph);

  /*! pure virtual, reimplement it on others, this will be the action that will happen in the graph 
     \param pos the position on screen that the execution will take part.
  */
  virtual void executePress(QPointF pos);
  virtual void executeMove(QPointF pos);
  virtual void executeRelease(QPointF pos);

  /*! sends this action to be the executed thing with mouse interaction on the GraphicsView */
  void sendExecuteBit();

protected:
  libgraph::GraphDocument *_graphDocument;  /*! pointer to the active graph document */
  libgraph::Graph *_graph;	/*! pointer to the active graph */
  GraphScene *_graphScene;	/*! pointer to the GraphicsScene with the drawing */

};

#endif
