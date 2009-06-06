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
#include <QGraphicsItem>
#include "graph.h"
#include "graphDocument.h"
#include "node.h"
#include "edge.h"
#include "SUI_MainWindow.h"
#include "graphicsitem_Edge.h"
#include "graphicsitem_Node.h"
#include "graphicsitem_OrientedEdge.h"

GraphPropertiesWidget::GraphPropertiesWidget ( MainWindow* parent )
        : QWidget ( parent ) {

    setupUi(this);
    unsetAll();
    _model = new GraphPropertiesModel();
    _tableView->setModel(_model);
    _mainWindow = parent;
    
  connect(_nodeName,  SIGNAL(textChanged(QString)), this, SLOT(nodeNameChanged(QString)) );
  connect(_nodeColor, SIGNAL(activated(QColor)),     this, SLOT(nodeColorChanged(QColor)) );
  connect(_nodeEnd,   SIGNAL(toggled(bool)),        this, SLOT(nodeEndChanged(bool))     );
  connect(_nodeBegin, SIGNAL(toggled(bool)),        this, SLOT(nodeBeginChanged(bool))   );
  connect(_nodeX,     SIGNAL(valueChanged(int)),    this, SLOT(nodeXChanged(int))        );
  connect(_nodeY,     SIGNAL(valueChanged(int)),    this, SLOT(nodeYChanged(int))        );
  connect(_nodeValue, SIGNAL(textChanged(QString)), this, SLOT( nodeValueChanged(QString)));
  connect(_edgeColor, SIGNAL(activated(QColor)),    this, SLOT(edgeColorChanged(QColor)) );
  connect(_edgeName, SIGNAL(textChanged(QString)),  this, SLOT(edgeNameChanged(QString)));
  connect(_edgeValue, SIGNAL(textChanged(QString)), this, SLOT(edgeValueChanged(QString)));
}

void GraphPropertiesWidget::setDataSource(QGraphicsItem *o) {
   if (GraphDocument *g = _mainWindow->activeDocument()){
 
      _nodeX->setRange(0,g->width());
      _nodeY->setRange(0, g->height());
    }

    QObject *obj = 0;
    unsetAll();
    if ( o == 0){
      if (Graph *g = _mainWindow->graph()){
	 obj = g;
	 setGraph(g);
	 _graphicsItem = 0;
      }
    }
    else if( NodeItem *nodeItem = qgraphicsitem_cast<NodeItem*>(o) ){
      _graphicsItem = nodeItem;      
      obj = nodeItem->node();
      setNode(nodeItem->node());
    }
    else if( EdgeItem *edgeItem = qgraphicsitem_cast<EdgeItem*>(o)){
      _graphicsItem = edgeItem;
      obj = edgeItem->edge();
      setEdge(edgeItem->edge());
    }
    else if ( OrientedEdgeItem *edgeItem = qgraphicsitem_cast<OrientedEdgeItem*>(o)){
      _graphicsItem = edgeItem;
      obj = edgeItem->edge();
      setEdge(edgeItem->edge());
    } 
    _model->setDataSource(obj);
}

void GraphPropertiesWidget::unsetAll(){
  unsetGraph();
  unsetNode();
  unsetEdge();
}

void GraphPropertiesWidget::setNode(Node *n){
   kDebug() << "Node Setted!";
  _node = n;
  kDebug() << n->name() << n->color().toAscii() << n->end() << n->begin() << n->x() << n->y() << n->value(); 
  
  _nodePAnel->show();
  _nodeName->setText(_node->name());
  _nodeColor->setColor(_node->color());
  _nodeEnd->setChecked(_node->end());
  _nodeBegin->setChecked(_node->begin());
  _nodeValue->setText(_node->value());
  _nodeX->setValue(_node->x());
  _nodeY->setValue(_node->y());
  
}

void GraphPropertiesWidget::setEdge(Edge *e){

}

void GraphPropertiesWidget::setGraph(Graph *g){

}

void GraphPropertiesWidget::unsetNode(){
  _node = 0;
  _graphicsItem = 0;
  _nodePAnel->hide();
}

void GraphPropertiesWidget::unsetGraph(){
  _graph = 0;
  _graphPanel->hide();
}

void GraphPropertiesWidget::unsetEdge(){
  _edge = 0;
  _graphicsItem = 0;
  _edgePanel->hide();
}

void GraphPropertiesWidget::nodeNameChanged(QString s){
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

void GraphPropertiesWidget::nodeValueChanged(QString s){
  if (!_node) return;
  _node->setValue(s);
  _graphicsItem->update();
}