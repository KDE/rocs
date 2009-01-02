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

#ifndef SUI_GRAPH_EDIT_WIDGET_H
#define SUI_GRAPH_EDIT_WIDGET_H

#include <QWidget>
#include "ui_GraphEditWidget.h"

namespace KTextEditor{
  class View;
  class Editor;
  class Document;
}


class GraphDocument;
class Graph;
class Node;
class Edge;
class GraphScene;
class MainWindow;

class GraphEditWidget : public QWidget, public Ui::GraphEditWidget{
Q_OBJECT  
public:
  /*! Default Constructor 
    \param parent the owner of this widget.  */
  GraphEditWidget(MainWindow *parent = 0);

  /*! Change the Script Editor Document. 
    \param d the new document.  */
  void setDocument ( KTextEditor::Document *d );

  /*! Gets the scene of the drawned graph.
   \return the GraphScene of the graph. */
  GraphScene *scene() const;

  /*! set the current selected Graph. 
      \param graph the new active graph.
  */
  void setGraph( Graph *graph);

public slots:
  /*! reset the graph view and insert the graph of the new document on it.
    \param gd the new graph document that will be shown on screen. */
  void setGraphDocument( GraphDocument *gd);

  /*! not implemented. */
  void removeNode(int i);

  /*! not implemented. */
  void removeEdge(int e);

  /*! not implemented. */
  void graphColorChanged(const QColor& c);

  /*! not implemented. */
  void on__btnRunScript_clicked();

private:
  /*! as the name says, draw a graph on scene.
  \param g the graph to be drawn. */
  void drawGraphOnScene( Graph *g);

  /*! as the name says, connect the graph signals to the scene. 
   \param g the graph that will be connected. 
  */
  void connectGraphSignals( Graph *g);

  /*! as the name says, it removes the current GraphDocument 
  from the screen and releases it from the evil hands of GraphEditWidget.*/
  void releaseGraphDocument();

  KTextEditor::Editor *_editor;
  KTextEditor::View *_txtEditScriptView;
  
  KTextEditor::Document *_txtEditScriptDocument;
  GraphScene *_graphScene;

   GraphDocument *_graphDocument;
   Graph *_graph;

  qreal topNode;
  qreal bottomNode;
  qreal leftNode;
  qreal rightNode;
};

#endif
