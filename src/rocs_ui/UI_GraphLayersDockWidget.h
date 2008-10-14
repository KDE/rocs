
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

#ifndef UI_GRAPH_LAYERS_DOCK_WIDGET_H
#define UI_GRAPH_LAYERS_DOCK_WIDGET_H

#include <QWidget>
#include <QListView>

#include "ui_GraphLayersDockWidget.h"

namespace libgraph{
  class GraphDocument;
  class Graph;
}
class GraphLayersModel;


/*! 
  \brief The Graph-Layer widget.
  This Holds in a layer-way (like Photoshop Layers) the Graphs of the current opened Graph File 
*/
class GraphLayersDockWidget : public QDockWidget, public Ui::GraphLayersDockWidget{
  Q_OBJECT
  public:
    GraphLayersDockWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

  public slots:
    void setGraphDocument(libgraph::GraphDocument *document);
    void setActiveGraph(const QModelIndex& modelindex);
    void on__btnNewGraph_clicked();

  signals:
    void activeGraphChanged(libgraph::Graph *graph);
    

  private:
    libgraph::GraphDocument *_document;
    GraphLayersModel *_layerModel;
};

#endif
