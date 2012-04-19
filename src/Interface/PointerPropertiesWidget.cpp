/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PointerPropertiesWidget.h"
#include "Pointer.h"
#include "MainWindow.h"
#include "model_GraphProperties.h"
#include <DataStructure.h>
#include <DataStructurePluginManager.h>

PointerPropertiesWidget::PointerPropertiesWidget(MainWindow *parent): QWidget(parent)
{
    setupUi(this);
    _pointer = PointerPtr();
}

void PointerPropertiesWidget::setPointer(PointerPtr e, QPointF pos)
{
    if (_pointer) {
        disconnectPointer();
    }
    _pointer = e;
    move(pos.x() + 10,  pos.y() + 10);

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_pointer.get());

    _propertiesTable->setModel(model);

    connect(_pointer.get(),      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    connect(_value,     SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setValue(QString)));
    connect(_name,      SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setName(QString)));
    connect(_width,     SIGNAL(valueChanged(double)),  this, SLOT(setWidth(double)));
    connect(_showName,  SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideName(bool)));
    connect(_showValue, SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideValue(bool)));

    reflectAttributes();
}

void PointerPropertiesWidget::setActive(bool active)
{
    if (active) {
        setVisible(true);
        activateWindow();
        raise();
        return;
    } else {
        setVisible(false);
    }
}

void PointerPropertiesWidget::setWidth(double v)
{
    _pointer->setWidth(static_cast<qreal>(v));
}

void PointerPropertiesWidget::reflectAttributes()
{
    if (_extraProperties->layout()) {
        delete _extraProperties->layout();
    }
    if (QLayout * lay = DataStructurePluginManager::self()->pointerExtraProperties(_pointer, this)) {
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

void PointerPropertiesWidget::on__color_activated(const QColor& c)
{
    _pointer->setColor(c.name());
}

void PointerPropertiesWidget::on__style_activated(int index)
{
    switch (index) {
    case 0 : _pointer->setStyle("solid");    break;
    case 1 : _pointer->setStyle("dash");     break;
    case 2 : _pointer->setStyle("dot");      break;
    case 3 : _pointer->setStyle("dash dot"); break;
    }
}

void PointerPropertiesWidget::on__addProperty_clicked()
{

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(_propertiesTable->model());
    model->addDynamicProperty(_propertyName->text(), QVariant(_propertyValue->text()),
                              _pointer.get(), (_isPropertyGlobal->checkState() == Qt::Checked));
}

void PointerPropertiesWidget::disconnectPointer()
{
    disconnect(_pointer.get(),      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    disconnect(_value,     SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setValue(QString)));
    disconnect(_name,      SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setName(QString)));
    disconnect(_width,     SIGNAL(valueChanged(double)),    this, SLOT(setWidth(double)));
    disconnect(_showName,  SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideName(bool)));
    disconnect(_showValue, SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideValue(bool)));
}
