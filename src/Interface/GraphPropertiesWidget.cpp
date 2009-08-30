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
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/

#include "GraphPropertiesWidget.h"
#include "model_GraphProperties.h"
#include <KDebug>
#include <QVariant>
#include <QGraphicsItem>
#include "graph.h"
#include "graphDocument.h"
#include "node.h"
#include "edge.h"
#include "MainWindow.h"
#include "EdgeItem.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"

GraphPropertiesWidget::GraphPropertiesWidget (Graph *g, MainWindow* parent )
        : QWidget ( parent ) {

   setupUi(this);
  _mainWindow = parent;

  _graph = g;
  _graphName->setText(_graph->name());
  _graphEdgeColor->setColor(_graph->edgeDefaultColor());
  _graphNodeColor->setColor(_graph->nodeDefaultColor());
  _graphAutomate->setChecked(_graph->automate());
  _graphAutomate->setChecked( _graph->directed());
  
}

void GraphPropertiesWidget::on__graphAutomate_clicked(bool b){

}

void GraphPropertiesWidget::on__graphEdgeApplyNow_cliicked(){
  
}

void GraphPropertiesWidget::on__graphEdgeColor_activated(QColor c){

}

void GraphPropertiesWidget::on__graphName_textChanged(QString n){

}

void GraphPropertiesWidget::on__graphNodeColorApply_Now_clicked(){

}

void GraphPropertiesWidget::on__graphNodeColor_activated(QColor c){

}

void GraphPropertiesWidget::on__graphOriented_clicked(bool b){

}

void GraphPropertiesWidget::on__graphDelete_clicked(){

}