
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

#ifndef SUI_GRAPH_LAYERS_DOCK_WIDGET_H
#define SUI_GRAPH_LAYERS_DOCK_WIDGET_H

#include <QWidget>
#include <QDockWidget>
#include <QListView>

#include "ui_GraphLayersDockWidget.h"

class GraphDocument;
class Graph;

class GraphLayersModel;


/*! 
  \brief The Graph-Layer widget.
  This Holds in a layer-way (like Photoshop Layers) the Graphs of the current opened Graph File 
*/
class GraphLayersDockWidget : public QDockWidget, public Ui::GraphLayersDockWidget{
  Q_OBJECT
  public:
    /*! public constructor */
    explicit GraphLayersDockWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  public slots:
    /*! send the GraphDocument to the list of graphs. 
      \param document the GraphDocument that will populate the model of the view.    */
    void setGraphDocument(GraphDocument *document);

    /*! sets the modelindex data ( that holds a graph ) to be the active Graph being editted on the canvas. 
    \param modelindex the index of the graph onto the model. 
    */
    void setActiveGraph(const QModelIndex& modelindex);

    /* executed when clicking the btnNewGraph, 
    this method creates a new graph. */
    void on__btnNewGraph_clicked();

    /* executed when clicking the btnNewOrientedGraph 
    this method creaates a new OrientedEdge Graph */
    void on__btnNewOrientedGraph_clicked();

  signals:
    /* Signal emmited when the active graph changes */
    void activeGraphChanged(Graph *graph);
    

  private:
    GraphDocument *_document;
    GraphLayersModel *_layerModel;
};

#endif
