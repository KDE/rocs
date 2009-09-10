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

EdgePropertiesWidget::EdgePropertiesWidget(): QWidget(0){
  setupUi(this);
}

void EdgePropertiesWidget::setEdge(Edge *e){
  _edge = e;
  _name->setText("Weeee");
//   _name->setText(_edge->name());
//   _value->setText(_edge->value());
//   _color->setColor(_edge->color());
//   _width->setValue(_edge->width());
//   _style->setCurrentItem("Solid");
}

void EdgePropertiesWidget::on__name_textChanged(const QString& s){
  _edge->setName(s);
}

void EdgePropertiesWidget::on__value_textChanged(const QString& s){
  _edge->setValue(s);
}

void EdgePropertiesWidget::on__color_activated(const QColor& c){
  _edge->setColor(c.name());
}

void EdgePropertiesWidget::on__width_valueChanged(double v){
  //_edge->setWidth(v);
}

void EdgePropertiesWidget::on__style_activated(const QString& s){
  //_edge->setStyle(s);
}