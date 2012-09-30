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
#include "model_GraphProperties.h"
#include "DataStructure.h"
#include "Actions/PropertiesDialogAction.h"
#include <DataStructurePluginManager.h>

PointerPropertiesWidget::PointerPropertiesWidget(PointerPtr pointer, QWidget* parent)
    : KDialog(parent)
{
    ui = new Ui::PointerPropertiesWidget;
    ui->setupUi(mainWidget());

    // edit data types by separate dialog
    QPointer<PropertiesDialogAction> dataTypePropertiesAction = new PropertiesDialogAction(
            i18n("Edit Pointer Types"), pointer->dataStructure()->document()->pointerType(pointer->pointerType()), this);
    ui->_editType->setDefaultAction(dataTypePropertiesAction);
    ui->_editType->setIcon(KIcon("document-properties"));
    connect(pointer->dataStructure()->document(), SIGNAL(dataTypeCreated(int)), this, SLOT(updatePointerTypes()));
    connect(pointer->dataStructure()->document(), SIGNAL(dataTypeRemoved(int)), this, SLOT(updatePointerTypes()));

    setCaption(i18nc("@title:window", "Pointer Properties"));
    setButtons(Close);
    setAttribute(Qt::WA_DeleteOnClose);
    setPointer(pointer);
}


void PointerPropertiesWidget::setPointer(PointerPtr pointer)
{
    PointerTypePtr pointerType;
    if (_pointer == pointer) {
        return;
    }
    if (_pointer) {
        pointerType = _pointer->dataStructure()->document()->pointerType(_pointer->pointerType());
        pointerType->disconnect(this);
        _pointer->disconnect(this);
        ui->_pointerType->clear();
    }
    _pointer = pointer;

    updatePointerTypes();

    // listen to pointer changes
    connect(_pointer.get(),      SIGNAL(changed()),         this, SLOT(reflectAttributes()));

    // connect ui
    connect(ui->_pointerType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setPointerType(int)));
    connect(ui->_width, SIGNAL(valueChanged(double)), this, SLOT(setWidth(double)));
    connect(ui->_color, SIGNAL(activated(QColor)), this, SLOT(setColor(QColor)));

    // listen to pointer type changes
    pointerType = _pointer->dataStructure()->document()->pointerType(_pointer->pointerType());
    connect(pointerType.get(), SIGNAL(propertyAdded(QString,QVariant)), this, SLOT(updateProperties()));
    connect(pointerType.get(), SIGNAL(propertyRemoved(QString)), this, SLOT(updateProperties()));
    connect(pointerType.get(), SIGNAL(propertyRenamed(QString,QString)), this, SLOT(updateProperties()));

    reflectAttributes();

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_pointer.get());

    ui->_propertiesTable->setModel(model);
    ui->_propertiesTable->horizontalHeader()->setProperty("stretchLastSection", true);
}

void PointerPropertiesWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10, screenPosition.y() + 10);
}

void PointerPropertiesWidget::reflectAttributes()
{
    if (!ui->extraItems->layout()) {
        ui->extraItems->setLayout(DataStructurePluginManager::self()->pointerExtraProperties(_pointer, this));
    }

    ui->_color->setColor(_pointer->color());
    ui->_width->setValue(_pointer->width());

    PointerTypePtr pointerType = _pointer->dataStructure()->document()->pointerType(_pointer->pointerType());
    ui->_pointerType->setCurrentIndex(ui->_pointerType->findData(QVariant(_pointer->pointerType())));
}

void PointerPropertiesWidget::setWidth(double v)
{
    _pointer->setWidth(static_cast<qreal>(v));
}


void PointerPropertiesWidget::setColor(const QColor& c)
{
    _pointer->setColor(c.name());
}

void PointerPropertiesWidget::setPointerType(int pointerTypeIndex)
{
    _pointer->setPointerType(ui->_pointerType->itemData(pointerTypeIndex).toInt());
}

void PointerPropertiesWidget::updatePointerTypes()
{
    ui->_pointerType->clear();
    // setup data types combobox
    foreach (int type, _pointer->dataStructure()->document()->pointerTypeList()) {
        QString typeString = _pointer->dataStructure()->document()->pointerType(type)->name();
        ui->_pointerType->addItem(typeString, QVariant(type));
    }
    if (_pointer) {
        ui->_pointerType->setCurrentIndex(ui->_pointerType->findData(QVariant(_pointer->pointerType())));
    }
}

void PointerPropertiesWidget::updateProperties()
{
    // TODO the following can be solved much nicer by updating the model
    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_pointer.get());
    ui->_propertiesTable->model()->deleteLater();
    ui->_propertiesTable->setModel(model);
}
