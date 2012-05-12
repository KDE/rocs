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
#include "model_GraphProperties.h"
#include <QPainter>

#include <DataStructurePluginManager.h>

DataPropertiesWidget::DataPropertiesWidget(DataPtr data, QWidget* parent)
    : KDialog(parent)
{
    ui = new Ui::DataPropertiesWidget;
    ui->setupUi(mainWidget());
    setCaption(i18n("Data Element Properties"));
    setButtons(Close); //TODO implement changes for (Ok | Cancel)

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->exportNewType, SIGNAL(clicked()), SLOT(addDataType()));
    setData(data);

    if (!_data->dataStructure()->document()->iconPackage().isEmpty()) {
        QFile svgFile(_data->dataStructure()->document()->iconPackage());
        svgFile.open(QIODevice::ReadOnly | QIODevice::Text);

        QXmlStreamReader reader(&svgFile);
        QSvgRenderer *renderer = DataItem::sharedRenderer(svgFile.fileName());
        while (!reader.atEnd()) {
            reader.readNext();
            if (!reader.attributes().hasAttribute("id")) {
                continue;
            }
            QString attribute = reader.attributes().value("id").toString();
            if (attribute.startsWith(QLatin1String("rocs_"))) {
                QImage iconImage = QImage(80, 80, QImage::Format_ARGB32);

                QPainter painter;
                painter.begin(&iconImage);
                renderer->render(&painter, attribute);
                painter.end();

                attribute.remove("rocs_");
                ui->dataTypeIcon->addItem(KIcon(QPixmap::fromImage(iconImage)), "", QVariant(attribute));
            }
        }
        if (!_data->dataStructure()->document()->dataType(_data->dataType())->iconName().isEmpty()) {
            QString icon = _data->dataStructure()->document()->dataType(_data->dataType())->iconName();
            icon.remove("rocs_");
            ui->dataTypeIcon->setCurrentIndex(ui->dataTypeIcon->findData(icon));
        }
        connect(ui->dataTypeIcon, SIGNAL(activated(int)),
                this, SLOT(setIcon(int)));
    }
}


void DataPropertiesWidget::setData(DataPtr data)
{
    if (_data == data) {
        return;
    }
    if (_data) {
        _data->disconnect(this);
        ui->_showName->disconnect(_data.get());
        ui->_showValue->disconnect(_data.get());
        ui->_enableColor->disconnect(_data.get());
        ui->_name->disconnect(_data.get());
        ui->_value->disconnect(_data.get());
        ui->_dataType->clear();
    }

    _data = data;

    // setup data types combobox
    foreach (int dataType, _data->dataStructure()->document()->dataTypeList()) {
        QString dataTypeString = _data->dataStructure()->document()->dataType(dataType)->name();
        ui->_dataType->addItem(dataTypeString, QVariant(dataType));
    }

    delete ui->extraItens->layout();
    ui->extraItens->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    reflectAttributes();

    connect(ui->_showName, SIGNAL(toggled(bool)),
            _data.get(), SLOT(setShowName(bool)));
    connect(ui->_showValue, SIGNAL(toggled(bool)),
            _data.get(), SLOT(setShowValue(bool)));
    connect(ui->_enableColor, SIGNAL(toggled(bool)),
            this, SLOT(setUseColor(bool)));
    connect(ui->_name, SIGNAL(textEdited(QString)),
            _data.get(), SLOT(setName(QString)));
    connect(ui->_value, SIGNAL(textEdited(QString)),
            _data.get(), SLOT(setValue(QString)));
    connect(ui->_dataType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setDataType(int)));
    connect(ui->_color, SIGNAL(activated(QColor)),
            this, SLOT(colorChanged(QColor)));
    connect(ui->_addProperty, SIGNAL(clicked(bool)),
            this, SLOT(addProperty()));

    GraphPropertiesModel *model = new GraphPropertiesModel();
    model->setDataSource(_data.get());

    ui->_propertiesTable->setModel(model);
}


void DataPropertiesWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10, screenPosition.y() + 10);
}


void DataPropertiesWidget::setUseColor(bool b)
{
    _data->setUseColor(b);
}


void DataPropertiesWidget::reflectAttributes()
{
    if (!ui->extraItens->layout()) {
        _oldDataStructurePlugin = DataStructurePluginManager::self()->pluginName();
    }

    if (_oldDataStructurePlugin != DataStructurePluginManager::self()->pluginName()) {
        ui->extraItens->layout()->deleteLater();
    }

    if (!ui->extraItens->layout()) {
        ui->extraItens->setLayout(DataStructurePluginManager::self()->dataExtraProperties(_data, this));
    }

    ui->_color->setColor(_data->color().value<QColor>());
    ui->_name->setText(_data->name());
    ui->_value->setText(_data->value().toString());
    ui->_showName->setChecked(_data->showName());
    ui->_showValue->setChecked(_data->showValue());
    ui->_enableColor->setChecked(_data->useColor());

    DataTypePtr dataType = _data->dataStructure()->document()->dataType(_data->dataType());
    ui->_dataType->setCurrentIndex(ui->_dataType->findData(QVariant(_data->dataType())));
    ui->dataTypeIcon->setCurrentIndex(ui->dataTypeIcon->findData(QVariant(dataType->iconName())));
}


void DataPropertiesWidget::colorChanged(const QColor& c)
{
    _data->setColor(QColor(c));
}


void DataPropertiesWidget::setDataType(int dataTypeIndex)
{
    _data->setDataType(ui->_dataType->itemData(dataTypeIndex).toInt());
    ui->dataTypeIcon->setCurrentIndex(ui->dataTypeIcon->findData(QVariant(_data->icon())));
}


void DataPropertiesWidget::addProperty()
{
    GraphPropertiesModel *model = qobject_cast< GraphPropertiesModel*>(ui->_propertiesTable->model());
    model->addDynamicProperty(i18n("untitled %1", model->rowCount()), 0, _data.get(), false);
}


void DataPropertiesWidget::addDataType()
{
    if (ui->newTypeName->text().isEmpty()){
        return;
    }

    DataStructurePtr ds = _data->dataStructure();
    int dataTypeIdentifier = ds->document()->registerDataType(ui->newTypeName->text());
    DataTypePtr datatype = ds->document()->dataType(dataTypeIdentifier);

    QString iconName = ui->dataTypeIcon->itemData(ui->dataTypeIcon->currentIndex()).toString();
    datatype->setIcon(iconName);

    ui->_dataType->addItem(datatype->name(), QVariant(dataTypeIdentifier));
    ui->_dataType->setCurrentIndex(ui->_dataType->findData(QVariant(dataTypeIdentifier)));
    ui->dataTypeIcon->setCurrentItem(datatype->iconName());
}


void DataPropertiesWidget::setIcon(int index)
{
    int dataTypeValue = _data->dataType();
    _data->dataStructure()->document()->dataType(dataTypeValue)->setIcon(
            ui->dataTypeIcon->itemData(index).toString()
            );
}
