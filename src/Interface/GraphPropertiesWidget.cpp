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
#include "NodeItem.h"
#include "OrientedEdgeItem.h"
#include "GraphScene.h"
#include <KLocale>
#include <QRadioButton>
#include <DSPluginManager.h>


GraphPropertiesWidget::GraphPropertiesWidget (Graph *g, MainWindow* parent )
        : KButtonGroup ( parent ) {
    setupUi(this);
    _mainWindow = parent;
    //! do not lock here, it will create a racing condition.

    _graph = g;
    _graphName->setText(_graph->name());
    _graphEdgeColor->setColor(_graph->edgeDefaultColor());
    _graphNodeColor->setColor(_graph->nodeDefaultColor());
//     _graphAutomate->setChecked(_graph->automate());
//     _graphOriented->setChecked(_graph->directed());
    _graphVisible->setChecked( ! _graph->readOnly());
    _activateGraph->setChecked(true);
    _showEdgeNames->setChecked( _graph->edgeNameVisibility() );
    _showEdgeValues->setChecked(_graph->edgeValueVisibility());
    _showNodeNames->setChecked( _graph->nodeNameVisibility() );
    _showNodeValues->setChecked(_graph->nodeValueVisibility());

    _editWidget->setVisible(_activateGraph->isChecked());

    if (_extraProperties->layout()){
      delete _extraProperties->layout();
    }
    if (QLayout * lay = Rocs::DSPluginManager::New()->graphExtraProperties(g, _extraProperties)){
        _extraProperties->setLayout(lay);
    }

    GraphDocument *gDocument = qobject_cast<GraphDocument*>(g->parent());
    connect(this, SIGNAL(addGraph(QString)), gDocument, SLOT(addGraph(QString)));
    connect(this, SIGNAL(removeGraph()), g, SLOT(remove()));

    connect( _graphEdgeColor, SIGNAL(activated(QColor)), this, SLOT(setEdgeDefaultColor(QColor)));
    connect( _graphNodeColor, SIGNAL(activated(QColor)), this, SLOT(setNodeDefaultColor(QColor)));

    connect( this, SIGNAL( edgeColorsChanged(QString)),      g, SLOT(setEdgesColor(QString)));
    connect( this, SIGNAL( nodeColorsChanged(QString)),      g, SLOT(setNodesColor(QString)));
    connect( this, SIGNAL( edgeDefaultColorSetted(QString)), g, SLOT(setEdgeDefaultColor(QString)));
    connect( this, SIGNAL( nodeDefaultColorSetted(QString)), g, SLOT(setNodeDefaultColor(QString)));


    connect( _showEdgeNames,  SIGNAL(toggled(bool)), g, SLOT(setEdgeNameVisibility(bool)  ));
    connect( _showEdgeValues, SIGNAL(toggled(bool)), g, SLOT(setEdgeValueVisibility(bool) ));
    connect( _showNodeNames,  SIGNAL(toggled(bool)), g, SLOT(setNodeNameVisibility(bool)  ));
    connect( _showNodeValues, SIGNAL(toggled(bool)), g, SLOT(setNodeValueVisibility(bool) ));

    connect( _graphName,      SIGNAL(textChanged(QString)), g, SLOT(setName(QString)));



}

void GraphPropertiesWidget::setEdgeDefaultColor(QColor c){    emit edgeDefaultColorSetted(c.name()); }
void GraphPropertiesWidget::setNodeDefaultColor(QColor c){    emit nodeDefaultColorSetted(c.name()); }
void GraphPropertiesWidget::on__graphEdgeColorApplyNow_clicked() {  emit edgeColorsChanged(_graphEdgeColor->color().name()); }
void GraphPropertiesWidget::on__graphNodeColorApplyNow_clicked() {  emit nodeColorsChanged(_graphNodeColor->color().name()); }

void GraphPropertiesWidget::on__graphVisible_toggled(bool b){
  _mainWindow->mutex().lock();
  _graph->readOnly( !b );
  _mainWindow->scene()->hideGraph( _graph, b );
  _mainWindow->mutex().unlock();
}

QRadioButton *GraphPropertiesWidget::radio()const {
    return _activateGraph;
}

void GraphPropertiesWidget::on__activateGraph_toggled(bool b) {
    _editWidget->setVisible( b );
    if (b) {
       _mainWindow->setActiveGraph(_graph);
    }
}

void GraphPropertiesWidget::on__graphDelete_clicked() {
    if (! _mainWindow->mutex().tryLock())
        return;
    bool createNewGraph = false;
    bool isActive = false;

    if (_graph == _mainWindow->graph()){
      isActive = true;
    }

    GraphDocument *gd = qobject_cast<GraphDocument*>(_graph->parent());

    if (gd->size() == 1){
	createNewGraph = true;
    }


    if (isActive) emit updateNeeded();
    radio()->group()->removeButton(radio());
    _mainWindow->mutex().unlock();

        /*! remove this graph from the document. */
    emit removeGraph();

    if (createNewGraph){
	emit addGraph(i18n("Untitled0"));
    }

    deleteLater();
}

void GraphPropertiesWidget::on__graphName_textChanged(const QString& s){
    _activateGraph->setText(s);
}

