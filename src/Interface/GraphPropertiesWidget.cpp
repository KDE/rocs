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
#include <QGraphicsItem>
#include "graph.h"
#include "graphDocument.h"
#include "node.h"
#include "edge.h"
#include "MainWindow.h"
#include "EdgeItem.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"
#include "GraphScene.h"

GraphPropertiesWidget::GraphPropertiesWidget (Graph *g, MainWindow* parent )
        : KButtonGroup ( parent ) {

    setupUi(this);
    _mainWindow = parent;

    _graph = g;
    _graphName->setText(_graph->name());
    _graphEdgeColor->setColor(_graph->edgeDefaultColor());
    _graphNodeColor->setColor(_graph->nodeDefaultColor());
    _graphAutomate->setChecked(_graph->automate());
    _graphOriented->setChecked(_graph->directed());
    _activateGraph->setChecked(true);

    _editWidget->setVisible(_activateGraph->isChecked());
    _mainWindow->setActiveGraph(_graph);
    kDebug() << "Active Graph" << _mainWindow->graph()->name();
    kDebug() << "This Graph" << _graph->name();
}
QRadioButton *GraphPropertiesWidget::radio()const {
    return _activateGraph;
}

void GraphPropertiesWidget::on__activateGraph_toggled(bool b) {
    _editWidget->setVisible(b);
    if (b) {
        _mainWindow->setActiveGraph(_graph);
    }
}

void GraphPropertiesWidget::on__graphEdgeColorApplyNow_clicked() {
    foreach(Edge *e, _graph->edges()) {
        e->setColor(_graphEdgeColor->color().name());
    }
}

void GraphPropertiesWidget::on__graphEdgeColor_activated(QColor c) {
    _graph->setEdgeDefaultColor(c.name());
}

void GraphPropertiesWidget::on__graphName_textChanged(QString n) {
    _activateGraph->setText(n);
    _graph->setName(n);
}

void GraphPropertiesWidget::on__graphNodeColorApplyNow_clicked() {
    foreach(Node *n, _graph->nodes()) {
        n->setColor(_graphNodeColor->color().name());
    }
}

void GraphPropertiesWidget::on__graphNodeColor_activated(QColor c) {
    _graph->setNodeDefaultColor(c.name());
}

void GraphPropertiesWidget::on__graphOriented_toggled(bool b) {
    _graph->setDirected(b);
    _mainWindow->scene()->updateDocument();
    kDebug() << "Toggle";
}

void GraphPropertiesWidget::on__graphAutomate_toggled(bool b) {
    _graph->setAutomate(b);
}

void GraphPropertiesWidget::on__graphDelete_clicked() {
  bool isActive = false;
  if (_graph == _mainWindow->graph()){
	isActive = true;
    }
    _graph->remove();
    if (isActive) emit updateNeeded();
    radio()->group()->removeButton(radio());
    deleteLater();
}

void GraphPropertiesWidget::on__showNodeNames_toggled(bool b) {
    foreach(Node *n, _graph->nodes()) {
        n->hideName(!b);
    }
}

void GraphPropertiesWidget::on__showEdgeNames_toggled(bool b) {
    foreach(Edge *e, _graph->edges()) {
        e->hideName(!b);
    }
}

void GraphPropertiesWidget::on__showNodeValues_toggled(bool b) {
    foreach(Node *n, _graph->nodes()) {
        n->hideValue(!b);
    }
}

void GraphPropertiesWidget::on__showEdgeValues_toggled(bool b) {
    foreach(Edge *e, _graph->edges()) {
        e->hideValue(!b);
    }
}
