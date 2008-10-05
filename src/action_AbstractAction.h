
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

class AbstractAction : public KAction
{
Q_OBJECT
public: 

  AbstractAction(int type, GraphScene *scene, QObject *parent);
  int type() { return _type; }

  void setGraphDocument(libgraph::GraphDocument *graphDocument);
  void setGraphScene(GraphScene *graphScene);

public slots:
  void setGraph(libgraph::Graph *graph);
  virtual void execute(int type, QPointF pos) = 0;
  void sendExecuteBit();

protected:

  int _type;
  libgraph::GraphDocument *_graphDocument; 
  libgraph::Graph *_graph;
  GraphScene *_graphScene;
};

#endif