/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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

#include "DataPropertiesWidget.h"
#include "Data.h"
#include "MainWindow.h"
#include <KDebug>
#include "DataItem.h"
#include "DataStructure.h"
#include "PropertiesDialogAction.h"
#include "model_GraphProperties.h"
#include <QPainter>

#include <DataStructurePluginManager.h>

DataPropertiesWidget::DataPropertiesWidget(DataPtr data, QWidget* parent)
    : KDialog(parent)
{
    ui = new Ui::DataPropertiesWidget;
    ui->setupUi(mainWidget());

    // edit data types by separate dialog
    QPointer<PropertiesDialogAction> dataTypePropertiesAction = new PropertiesDialogAction(
            i18n("Edit Data Types"), data->dataStructure()->document()->dataType(data->dataType()), this);
    ui->_editType->setDefaultAction(dataTypePropertiesAction);
    ui->_editType->setIcon(KIcon("document-properties"));
    connect(data->dataStructure()->document(), SIGNAL(dataTypeCreated(int)), this, SLOT(updateDataTypes()));
    connect(data->dataStructure()->document(), SIGNAL(dataTypeRemoved(int)), this, SLOT(updateDataTypes()));

    setCaption(i18n("Data Element Properties"));
    setButtons(Close);
    setAttribute(Qt::WA_DeleteOnClose);
    setData(data);
}


void DataPropertiesWidget::setData(DataPtr data)
{
    if (_data == data) {
        return;
    }
    if (_data) {
        _data->disconnect(this);
        ui->_dataType->clear();
    }

    _data = data;

    updateDataTypes();

    delete ui->extraItems->layout();
    ui->extraItems->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    reflectAttributes();

    connect(ui->_dataType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setDataType(int)));
    connect(ui->_color, SIGNAL(activated(QColor)),
            this, SLOT(colorChanged(QColor)));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_data.get());

    ui->_propertiesTable->setModel(model);
    ui->_propertiesTable->horizontalHeader()->setProperty("stretchLastSection", true);
}


void DataPropertiesWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10, screenPosition.y() + 10);
}


void DataPropertiesWidget::reflectAttributes()
{
    if (!ui->extraItems->layout()) {
        _oldDataStructurePlugin = DataStructurePluginManager::self()->pluginName();
    }

    if (_oldDataStructurePlugin != DataStructurePluginManager::self()->pluginName()) {
        ui->extraItems->layout()->deleteLater();
    }

    if (!ui->extraItems->layout()) {
        ui->extraItems->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    }

    ui->_color->setColor(_data->color().value<QColor>());

    DataTypePtr dataType = _data->dataStructure()->document()->dataType(_data->dataType());
    ui->_dataType->setCurrentIndex(ui->_dataType->findData(QVariant(_data->dataType())));
}


void DataPropertiesWidget::colorChanged(const QColor& c)
{
    _data->setColor(QColor(c));
}


void DataPropertiesWidget::setDataType(int dataTypeIndex)
{
    _data->setDataType(ui->_dataType->itemData(dataTypeIndex).toInt());
}

void DataPropertiesWidget::updateDataTypes()
{
    ui->_dataType->clear();
    // setup data types combobox
    foreach (int dataType, _data->dataStructure()->document()->dataTypeList()) {
        QString dataTypeString = _data->dataStructure()->document()->dataType(dataType)->name();
        ui->_dataType->addItem(dataTypeString, QVariant(dataType));
    }
    if (_data) {
        ui->_dataType->setCurrentIndex(ui->_dataType->findData(QVariant(_data->dataType())));
    }
}
