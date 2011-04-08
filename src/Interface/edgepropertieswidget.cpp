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
#include "Pointer.h"
#include "MainWindow.h"
#include "model_GraphProperties.h"
#include <DataStructure.h>
#include <DataStructurePluginManager.h>

PointerPropertiesWidget::PointerPropertiesWidget(MainWindow *parent): QWidget(parent) {
    setupUi(this);
    _pointer = 0;
}

void PointerPropertiesWidget::setPointer(Pointer *e, QPointF pos) {
    if (_pointer == e)
      return;

    if (_pointer){
      disconnectPointer();
    }
    _pointer = e;
    move(pos.x()+ 10,  pos.y() + 10);

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_pointer);

    _propertiesTable->setModel(model);


    show();
    activateWindow();
    raise();

    connect(_pointer,      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    connect(_value,     SIGNAL(textChanged(QString)),   _pointer, SLOT(setValue(QString)));
    connect(_name,      SIGNAL(textChanged(QString)),   _pointer, SLOT(setName(QString)));
    connect(_width,     SIGNAL(valueChanged(double)),  this, SLOT(setWidth(double)));
    connect(_showName,  SIGNAL(toggled(bool)),          _pointer, SLOT(hideName(bool)));
    connect(_showValue, SIGNAL(toggled(bool)),          _pointer, SLOT(hideValue(bool)));

    reflectAttributes();
}

void PointerPropertiesWidget::setWidth(double v)
{
    _pointer->setWidth( static_cast<qreal>(v));
}

void PointerPropertiesWidget::reflectAttributes(){
  if (_extraProperties->layout()){
    delete _extraProperties->layout();
  }
  if (QLayout * lay = DataStructurePluginManager::self()->pointerExtraProperties(_pointer, this)){
      _extraProperties->setLayout(lay);
  }
   _name->setText(_pointer->name());
   _value->setText(_pointer->value());
   _color->setColor(_pointer->color());
   _width->setValue(_pointer->width());
   _propertyName->setText("");
   _propertyValue->setText("");
   _isPropertyGlobal->setCheckState(Qt::Unchecked);
}

void PointerPropertiesWidget::on__color_activated(const QColor& c) {
    _pointer->setColor(c.name());
}

void PointerPropertiesWidget::on__style_activated(int index) {
    switch(index){
      case 0 : _pointer->setStyle("solid");    break;
      case 1 : _pointer->setStyle("dash");     break;
      case 2 : _pointer->setStyle("dot");      break;
      case 3 : _pointer->setStyle("dash dot"); break;
    }
}

void PointerPropertiesWidget::on__addProperty_clicked(){

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(), QVariant(_propertyValue->text()),
                              _pointer,(_isPropertyGlobal->checkState() == Qt::Checked));
}

void PointerPropertiesWidget::disconnectPointer(){
   disconnect(_pointer,      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    disconnect(_value,     SIGNAL(textChanged(QString)),   _pointer, SLOT(setValue(QString)));
    disconnect(_name,      SIGNAL(textChanged(QString)),   _pointer, SLOT(setName(QString)));
    disconnect(_width,     SIGNAL(valueChanged(double)),    _pointer, SLOT(setWidth(double)));
    disconnect(_showName,  SIGNAL(toggled(bool)),          _pointer, SLOT(hideName(bool)));
    disconnect(_showValue, SIGNAL(toggled(bool)),          _pointer, SLOT(hideValue(bool)));
}
