/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

PointerPropertiesWidget::PointerPropertiesWidget(PointerPtr pointer, QWidget* parent)
    : KDialog(parent)
{
    ui = new Ui::PointerPropertiesWidget;
    ui->setupUi(mainWidget());
    setCaption(i18n("Pointer Properties"));
    setButtons(Close); //TODO implement changes for (Ok | Cancel)
    setAttribute(Qt::WA_DeleteOnClose);

    connect(this, SIGNAL(okClicked()), SLOT(close()));
    setPointer(pointer);
}


void PointerPropertiesWidget::setPointer(PointerPtr e)
{
    if (_pointer) {
        disconnectPointer();
    }
    _pointer = e;

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_pointer.get());

    ui->_propertiesTable->setModel(model);

    connect(_pointer.get(),      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    connect(ui->_value,     SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setValue(QString)));
    connect(ui->_name,      SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setName(QString)));
    connect(ui->_width,     SIGNAL(valueChanged(double)),  this, SLOT(setWidth(double)));
    connect(ui->_showName,  SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideName(bool)));
    connect(ui->_showValue, SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideValue(bool)));

    reflectAttributes();
}


void PointerPropertiesWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10,  screenPosition.y() + 10);
}


void PointerPropertiesWidget::setWidth(double v)
{
    _pointer->setWidth(static_cast<qreal>(v));
}


void PointerPropertiesWidget::reflectAttributes()
{
    if (ui->_extraProperties->layout()) {
        delete ui->_extraProperties->layout();
    }
    if (QLayout * lay = DataStructurePluginManager::self()->pointerExtraProperties(_pointer, this)) {
        ui->_extraProperties->setLayout(lay);
    }
    ui->_name->setText(_pointer->name());
    ui->_value->setText(_pointer->value());
    ui->_color->setColor(_pointer->color());
    ui->_width->setValue(_pointer->width());
    ui->_propertyName->setText("");
    ui->_propertyValue->setText("");
    ui->_isPropertyGlobal->setCheckState(Qt::Unchecked);
}


void PointerPropertiesWidget::on__color_activated(const QColor& c)
{
    _pointer->setColor(c.name());
}


void PointerPropertiesWidget::on__addProperty_clicked()
{

    GraphPropertiesModel *model =  qobject_cast< GraphPropertiesModel*>(ui->_propertiesTable->model());
    model->addDynamicProperty(ui->_propertyName->text(), QVariant(ui->_propertyValue->text()),
                              _pointer.get());
}


void PointerPropertiesWidget::disconnectPointer()
{
    disconnect(_pointer.get(),      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    disconnect(ui->_value,     SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setValue(QString)));
    disconnect(ui->_name,      SIGNAL(textChanged(QString)),   _pointer.get(), SLOT(setName(QString)));
    disconnect(ui->_width,     SIGNAL(valueChanged(double)),    this, SLOT(setWidth(double)));
    disconnect(ui->_showName,  SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideName(bool)));
    disconnect(ui->_showValue, SIGNAL(toggled(bool)),          _pointer.get(), SLOT(hideValue(bool)));
}
