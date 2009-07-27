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

#include "SUI_GraphPropertiesWidget.h"
#include "model_GraphProperties.h"
#include <KDebug>
#include <QVariant>
#include <QGraphicsItem>
#include "graph.h"
#include "graphDocument.h"
#include "node.h"
#include "edge.h"
#include "SUI_MainWindow.h"
#include "EdgeItem.h"
#include "NodeItem.h"
#include "OrientedEdgeItem.h"

GraphPropertiesWidget::GraphPropertiesWidget ( MainWindow* parent )
        : QScrollArea ( parent ) {

    setupUi(this);
    unsetAll();
    _model = new GraphPropertiesModel();
    _tableView->setModel(_model);
    _mainWindow = parent;
    
  connect(_nodeName,  SIGNAL(textChanged(const QString&)), this, SLOT(nodeNameChanged(const QString&)) );
  connect(_nodeColor, SIGNAL(activated(QColor)),     this, SLOT(nodeColorChanged(QColor)) );
  connect(_nodeEnd,   SIGNAL(toggled(bool)),        this, SLOT(nodeEndChanged(bool))     );
  connect(_nodeBegin, SIGNAL(toggled(bool)),        this, SLOT(nodeBeginChanged(bool))   );
  connect(_nodeX,     SIGNAL(valueChanged(int)),    this, SLOT(nodeXChanged(int))        );
  connect(_nodeY,     SIGNAL(valueChanged(int)),    this, SLOT(nodeYChanged(int))        );
  connect(_nodeValue, SIGNAL(textChanged(const QString&)), this, SLOT( nodeValueChanged(const QString&)));
  connect(_edgeColor, SIGNAL(activated(QColor)),    this, SLOT(edgeColorChanged(QColor)) );
  connect(_edgeName, SIGNAL(textChanged(const QString&)),  this, SLOT(edgeNameChanged(const QString&)));
  connect(_edgeValue, SIGNAL(textChanged(const QString&)), this, SLOT(edgeValueChanged(const QString&)));
  
}

void GraphPropertiesWidget::setDataSource(QGraphicsItem *o) {
   if (GraphDocument *g = _mainWindow->activeDocument()){
       _nodeX->setRange(0,g->width());
      _nodeY->setRange(0, g->height());
    }
  
    unsetAll();
    _graphicsItem = o;

    if( NodeItem *nodeItem = qgraphicsitem_cast<NodeItem*>(o) ){
      setNode(nodeItem->node());
    }
    else if( EdgeItem *edgeItem = qgraphicsitem_cast<EdgeItem*>(o)){
      setEdge(edgeItem->edge());
    }
    else if ( OrientedEdgeItem *edgeItem = qgraphicsitem_cast<OrientedEdgeItem*>(o)){
      setEdge(edgeItem->edge());
    } 
    _model->setDataSource(_obj);
}

void GraphPropertiesWidget::unsetAll(){
  hide();
  unsetGraph();
  unsetNode();
  unsetEdge();
}

void GraphPropertiesWidget::setNode(Node *n){
  _node = n;
  _obj = n;
  _nodePAnel->show();
  _nodeName->setText(_node->name());
  _nodeColor->setColor(_node->color());
  _nodeEnd->setChecked(_node->end());
  _nodeBegin->setChecked(_node->begin());
  _nodeValue->setText(_node->value());
  _nodeX->setValue(_node->x());
  _nodeY->setValue(_node->y());
  show();
}

void GraphPropertiesWidget::setEdge(Edge *e){
  _edge = e;
  _obj = e;
  _edgePanel->show();
  _edgeName->setText(_edge->name());
  _edgeColor->setColor(_edge->color());
  _edgeValue->setText(_edge->value());
  show();
}

void GraphPropertiesWidget::setGraph(Graph *g){
  _obj = g;
}

void GraphPropertiesWidget::unsetNode(){
  _node = 0;
  _obj = 0;
  _graphicsItem = 0;
  _nodePAnel->hide();
}

void GraphPropertiesWidget::unsetGraph(){
  _graph = 0;
  _obj = 0;
  _graphPanel->hide();
}

void GraphPropertiesWidget::unsetEdge(){
  _edge = 0;
  _obj = 0;
  _graphicsItem = 0;
  _edgePanel->hide();
}

void GraphPropertiesWidget::on__btnAddProperty_clicked(){
  if (_obj == 0) return;
  if (_txtPropertyName->text().isEmpty()) return;
  _model->setDataSource(0);
  if (_txtPropertyValue->text().isEmpty()){
    _obj->setProperty(_txtPropertyName->text().toAscii(), QVariant());
  }
  else{
    _obj->setProperty(_txtPropertyName->text().toAscii(), _txtPropertyValue->text());
  }

   _model->setDataSource(_obj);
}

void GraphPropertiesWidget::nodeNameChanged(const QString& s){
  if (!_node) return;
  _node->setName(s);
  _graphicsItem->update();
}

void GraphPropertiesWidget::nodeColorChanged(QColor c){
  if (!_node) return;
  _node->setColor(c.name());
  _graphicsItem->update();
}

void GraphPropertiesWidget::nodeEndChanged(bool b){
  if (!_node) return;
  _node->setEnd(b);
  _graphicsItem->update();
}
void GraphPropertiesWidget::nodeBeginChanged(bool b){
  if (!_node) return;
  _node->setBegin(b);
  _graphicsItem->update();
}
void GraphPropertiesWidget::nodeXChanged(int x){
  if (!_node) return;
  if (NodeItem *n = qgraphicsitem_cast<NodeItem*>(_graphicsItem)){
    n->node()->setX(x);
  }
}
void GraphPropertiesWidget::nodeYChanged(int y){
  if (!_node) return;
  if (NodeItem *n = qgraphicsitem_cast<NodeItem*>(_graphicsItem)){
    n->node()->setY(y);
  }
}

void GraphPropertiesWidget::nodeValueChanged(const QString& s){
  if (!_node) return;
  _node->setValue(s);
  _graphicsItem->update();
}

void GraphPropertiesWidget::edgeValueChanged(const QString& s){
  if (!_edge) return;
  _edge->setValue(s);
  _graphicsItem->update();
}

void GraphPropertiesWidget::edgeNameChanged(const QString& s){
  if (!_edge) return;
  _edge->setName(s);
  _graphicsItem->update();
}

void GraphPropertiesWidget::edgeColorChanged(QColor c){
  if (!_edge) return;
  _edge->setColor(c.name());
  _graphicsItem->update();
}