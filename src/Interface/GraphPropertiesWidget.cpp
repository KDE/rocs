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
#include "DataStructure.h"
#include "Document.h"
#include "Data.h"
#include "Pointer.h"
#include "MainWindow.h"
#include "DataItem.h"
#include "PointerItem.h"
#include "GraphScene.h"
#include <KLocale>
#include <QRadioButton>
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"


GraphPropertiesWidget::GraphPropertiesWidget (DataStructure *g, MainWindow* parent )
        : KButtonGroup ( parent ) {
    setupUi(this);
    _mainWindow = parent;

    _graph = g;
    _graphName->setText(_graph->name());
    _graphEdgeColor->setColor(_graph->pointerDefaultColor());
    _graphNodeColor->setColor(_graph->dataDefaultColor());
    _graphVisible->setChecked( ! _graph->readOnly());
    _activateGraph->setChecked(true);
    _showEdgeNames->setChecked( _graph->pointerNameVisibility() );
    _showEdgeValues->setChecked(_graph->pointerValueVisibility());
    _showNodeNames->setChecked( _graph->dataNameVisibility() );
    _showNodeValues->setChecked(_graph->dataValueVisibility());

    _editWidget->setVisible(_activateGraph->isChecked());
    
    if (!_extraProperties->layout()){
        QLayout * lay = DataStructurePluginManager::self()->dataStructureExtraProperties(g, _extraProperties);
        _extraProperties->setLayout(lay);
    }

    Document *gDocument = qobject_cast<Document*>(g->parent());
    connect(this, SIGNAL(addGraph(QString)), gDocument, SLOT(addDataStructure(QString)));
    connect(this, SIGNAL(removeGraph()), g, SLOT(remove()));
    
    
    connect( _graphEdgeColor, SIGNAL(activated(QColor)), this, SLOT(setPointerDefaultColor(QColor)));
    connect( _graphNodeColor, SIGNAL(activated(QColor)), this, SLOT(setDatumDefaultColor(QColor)));

    connect( _showEdgeNames,  SIGNAL(toggled(bool)), g, SLOT(setPointerNameVisibility(bool)));
    connect( _showEdgeValues, SIGNAL(toggled(bool)), g, SLOT(setPointerValueVisibility(bool)));
    connect( _showNodeNames,  SIGNAL(toggled(bool)), g, SLOT(setDataNameVisibility(bool)  ));
    connect( _showNodeValues, SIGNAL(toggled(bool)), g, SLOT(setDataValueVisibility(bool) ));

    connect( _graphName,      SIGNAL(textChanged(QString)), g, SLOT(setName(QString)));
}

void GraphPropertiesWidget::setPointerDefaultColor(QColor c){ 
    _graph->setPointerDefaultColor(c); 
}

void GraphPropertiesWidget::setDatumDefaultColor(QColor c){
    _graph->setDataDefaultColor(c);
}

void GraphPropertiesWidget::on__graphPointerColorApplyNow_clicked() {  
    _graph->setPointersColor(_graphEdgeColor->color()); 
}

void GraphPropertiesWidget::on__graphDatumColorApplyNow_clicked() { 
    _graph->setDataColor(_graphNodeColor->color()); 
}

void GraphPropertiesWidget::on__graphVisible_toggled(bool b){
  _graph->setReadOnly( !b );
  _mainWindow->scene()->hideGraph( _graph, b );
}

QRadioButton *GraphPropertiesWidget::radio()const {
    return _activateGraph;
}

void GraphPropertiesWidget::on__activateGraph_toggled(bool b) {
    _editWidget->setVisible( b );
    if (b) {
       DocumentManager::self()->activeDocument()->setActiveDataStructure(_graph);
    }
}

void GraphPropertiesWidget::on__graphDelete_clicked() {
    bool createNewGraph = false;

    if (DocumentManager::self()->activeDocument()->dataStructures().size() == 1){
        createNewGraph = true;
    }

    radio()->group()->removeButton(radio());
    
    emit removeGraph();
    
    if (createNewGraph){
        emit addGraph(i18n("Untitled0"));
    }
    hide();
    deleteLater();
}

void GraphPropertiesWidget::on__graphName_textChanged(const QString& s){
    _activateGraph->setText(s);
}
