/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "edgepropertieswidget.h"
#include "edge.h"
#include "MainWindow.h"
#include "model_GraphProperties.h"
#include <graph.h>

EdgePropertiesWidget::EdgePropertiesWidget(MainWindow *parent): QWidget(parent) {
    setupUi(this);
    _edge = 0;
}

void EdgePropertiesWidget::setEdge(Edge *e, QPointF pos) {
    if (_edge == e)
      return;

    if (_edge){
      disconnectEdge();
    }
    _edge = e;
    move(pos.x()+ 10,  pos.y() + 10);

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_edge);

    _propertiesTable->setModel(model);


    show();
    activateWindow();
    raise();

    connect(_edge,      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    connect(_value,     SIGNAL(textChanged(QString)),   _edge, SLOT(setValue(QString)));
    connect(_name,      SIGNAL(textChanged(QString)),   _edge, SLOT(setName(QString)));
    connect(_width,     SIGNAL(valueChanged(double)),    _edge, SLOT(setWidth(double)));
    connect(_showName,  SIGNAL(toggled(bool)),          _edge, SLOT(hideName(bool)));
    connect(_showValue, SIGNAL(toggled(bool)),          _edge, SLOT(hideValue(bool)));

    reflectAttributes();
}

void EdgePropertiesWidget::reflectAttributes(){
   _name->setText(_edge->name());
   _value->setText(_edge->value());
   _color->setColor(_edge->color());
   _width->setValue(_edge->width());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
}

void EdgePropertiesWidget::on__color_activated(const QColor& c) {
    _edge->setColor(c.name());
}

void EdgePropertiesWidget::on__style_activated(int index) {
    switch(index){
      case 0 : _edge->setStyle("solid");    break;
      case 1 : _edge->setStyle("dash");     break;
      case 2 : _edge->setStyle("dot");      break;
      case 3 : _edge->setStyle("dash dot"); break;
    }
}

void EdgePropertiesWidget::on__addProperty_clicked(){
//     if (_isPropertyGlobal->checkState() == Qt::Checked ){
//       _edge->graph()->addEdgesDinamicProperty(_propertyName->text(),
// 					      QVariant(_propertyValue->text()));
//     }else{
//       _edge->addDinamicProperty(_propertyName->text(),
// 				QVariant(_propertyValue->text()));
//     }

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(), QVariant(_propertyValue->text()),
                              _edge,(_isPropertyGlobal->checkState() == Qt::Checked));
}

void EdgePropertiesWidget::disconnectEdge(){
   disconnect(_edge,      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    disconnect(_value,     SIGNAL(textChanged(QString)),   _edge, SLOT(setValue(QString)));
    disconnect(_name,      SIGNAL(textChanged(QString)),   _edge, SLOT(setName(QString)));
    disconnect(_width,     SIGNAL(valueChanged(double)),    _edge, SLOT(setWidth(double)));
    disconnect(_showName,  SIGNAL(toggled(bool)),          _edge, SLOT(hideName(bool)));
    disconnect(_showValue, SIGNAL(toggled(bool)),          _edge, SLOT(hideValue(bool)));
}
