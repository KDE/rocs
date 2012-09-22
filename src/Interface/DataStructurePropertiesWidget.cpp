/*
    This file is part of Rocs,
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
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

#include "DataStructurePropertiesWidget.h"

#include "model_GraphProperties.h"
#include "DataStructure.h"
#include "Document.h"
#include "Data.h"
#include "DataType.h"
#include "Pointer.h"
#include "PointerType.h"
#include "MainWindow.h"
#include "DataItem.h"
#include "PointerItem.h"
#include "GraphScene.h"
#include <DataStructurePluginManager.h>
#include "DocumentManager.h"
#include <PropertiesDialogAction.h>

#include <KLocale>
#include <KDebug>
#include <KAction>
#include <KColorCombo>
#include <KPushButton>

#include <QString>
#include <QGraphicsItem>
#include <QRadioButton>
#include <QMenu>


DataStructurePropertiesWidget::DataStructurePropertiesWidget(DataStructurePtr dataStructure, MainWindow* parent)
    : KButtonGroup(parent)
{
    setupUi(this);
    _mainWindow = parent;

    // create default data element setups
    foreach (int type, dataStructure->document()->dataTypeList()) {
        createDataTypeInformationWidget(type, dataStructure);
    }
    foreach (int type, dataStructure->document()->pointerTypeList()) {
        createPointerTypeInformationWidget(type, dataStructure);
    }


    _dataStructure = dataStructure;
    _dataStructureName->setText(_dataStructure->name());
//     _dataStructureVisible->setChecked( ! _dataStructure->readOnly());
    _activateGraph->setChecked(true);

    _displayOptions->setVisible(_activateGraph->isChecked());

    if (!_extraProperties->layout()) {
        QLayout * lay = DataStructurePluginManager::self()->dataStructureExtraProperties(dataStructure, _extraProperties);
        _extraProperties->setLayout(lay);
    }

    connect(this, SIGNAL(addGraph(QString)), dataStructure->document(), SLOT(addDataStructure(QString)));
    connect(this, SIGNAL(removeGraph(DataStructurePtr)), dataStructure.get(), SLOT(remove()));

    // react on new data types and pointer types
    connect(dataStructure->document(), SIGNAL(dataTypeCreated(int)), this, SLOT(registerDataType(int)));
    connect(dataStructure->document(), SIGNAL(dataTypeRemoved(int)), this, SLOT(unregisterDataType(int)));
    connect(dataStructure->document(), SIGNAL(pointerTypeCreated(int)), this, SLOT(registerPointerType(int)));
    connect(dataStructure->document(), SIGNAL(pointerTypeRemoved(int)), this, SLOT(unregisterPointerType(int)));
    connect(dataStructure->document(), SIGNAL(nameChanged(QString)), _dataStructureName, SLOT(setText(QString)));

    connect(_dataStructureName, SIGNAL(textChanged(QString)), dataStructure.get(), SLOT(setName(QString)));
}


DataStructurePropertiesWidget::~DataStructurePropertiesWidget()
{
}


void DataStructurePropertiesWidget::registerDataType(int identifier)
{
    createDataTypeInformationWidget(identifier, _dataStructure);
}


void DataStructurePropertiesWidget::unregisterDataType(int identifier)
{
    if (!_dataTypeWidgets.contains(identifier)) {
        return;
    }
    delete _dataTypeWidgets[identifier];
    _dataTypeWidgets.remove(identifier);
    _dataTypeButtons.remove(identifier);
}


void DataStructurePropertiesWidget::registerPointerType(int identifier)
{
    createPointerTypeInformationWidget(identifier, _dataStructure);
}


void DataStructurePropertiesWidget::unregisterPointerType(int identifier)
{
    if (!_pointerTypeWidgets.contains(identifier)) {
        return;
    }
    delete _pointerTypeWidgets[identifier];
    _pointerTypeWidgets.remove(identifier);
    _pointerTypeButtons.remove(identifier);
}


QRadioButton *DataStructurePropertiesWidget::radio()const
{
    return _activateGraph;
}


void DataStructurePropertiesWidget::on__activateGraph_toggled(bool visible)
{
    _displayOptions->setVisible(visible);
    if (visible) {
        DocumentManager::self()->activeDocument()->setActiveDataStructure(_dataStructure);
    }
}


void DataStructurePropertiesWidget::on__dataStructureDelete_clicked()
{
    emit removeGraph(_dataStructure);
}


bool DataStructurePropertiesWidget::createDataTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure)
{
    if (!dataStructure->document()->dataTypeList().contains(typeIdentifier)) {
        return false;
    }

    DataTypePtr dataType = dataStructure->document()->dataType(typeIdentifier);

    // create default data element setups
    QWidget* dataPropertyWidget = new QWidget(this);
    QGridLayout* dataPropertyLayout = new QGridLayout(dataPropertyWidget);
    KPushButton* dataTypeButton = new KPushButton(this);
    QMenu* dataTypeMenu = new QMenu(dataTypeButton);
    dataTypeButton->setFlat(true);
    dataTypeButton->setStyleSheet("text-align: left");
    dataTypeButton->setMenu(dataTypeMenu);
    dataTypeMenu->addAction(
            new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), dataType, dataPropertyWidget)
        );
    dataTypeButton->setIcon(dataType->icon());
    dataTypeButton->setText(dataType->name());

    KPushButton* dataTypeVisible = new KPushButton(this);
    dataTypeVisible->setIcon(KIcon("rocseyeblack"));
    dataTypeVisible->setCheckable(true);
    dataTypeVisible->setChecked(true);
    dataTypeVisible->setFixedWidth(24);

    dataPropertyWidget->setLayout(dataPropertyLayout);
    dataPropertyLayout->addWidget(dataTypeButton, 1, 1);
    dataPropertyLayout->addWidget(dataTypeVisible, 1, 2);

    _dataTypes->addWidget(dataPropertyWidget);

    QSignalMapper* signalMapper = new QSignalMapper(this);
    signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(dataTypeVisible, typeIdentifier);
    connect(dataTypeVisible, SIGNAL(toggled(bool)), signalMapper, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(int)), dataStructure.get(), SLOT(toggleDataVisibility(int)));

    connect(dataType.get(), SIGNAL(iconChanged(QString)), this, SLOT(updateDataTypeButtons()));
    connect(dataType.get(), SIGNAL(nameChanged(QString)), this, SLOT(updateDataTypeButtons()));

    _dataTypeWidgets.insert(typeIdentifier, dataPropertyWidget);
    _dataTypeButtons.insert(typeIdentifier, dataTypeButton);

    return true;
}


bool DataStructurePropertiesWidget::createPointerTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure)
{
    if (!dataStructure->document()->pointerTypeList().contains(typeIdentifier)) {
        return false;
    }

    PointerTypePtr pointerType = dataStructure->document()->pointerType(typeIdentifier);

    // create default data element setups
    QWidget* pointerPropertyWidget = new QWidget(this);
    QGridLayout* pointerPropertyLayout = new QGridLayout(pointerPropertyWidget);

    KPushButton* pointerTypeButton = new KPushButton(this);
    QMenu* pointerTypeMenu = new QMenu(pointerTypeButton);
    pointerTypeButton->setText(pointerType->name());
    pointerTypeButton->setFlat(true);
    pointerTypeButton->setStyleSheet("text-align: left");
    pointerTypeButton->setMenu(pointerTypeMenu);
    pointerTypeMenu->addAction(
            new PropertiesDialogAction(i18nc("@action:inmenu", "Properties"), pointerType, pointerPropertyWidget)
        );

    KPushButton* pointerTypeVisible = new KPushButton(this);
    pointerTypeVisible->setIcon(KIcon("rocseyeblack"));
    pointerTypeVisible->setCheckable(true);
    pointerTypeVisible->setChecked(true);
    pointerTypeVisible->setToolTip(i18nc("@info:tooltip", "Set visibility of pointers"));
    pointerTypeVisible->setFixedWidth(24);

    pointerPropertyWidget->setLayout(pointerPropertyLayout);
    pointerPropertyLayout->addWidget(pointerTypeButton, 1, 1);
    pointerPropertyLayout->addWidget(pointerTypeVisible, 1, 2);

    _pointerTypes->addWidget(pointerPropertyWidget);

    QSignalMapper* signalMapper = new QSignalMapper(this);
    signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(pointerTypeVisible, typeIdentifier);
    connect(pointerTypeVisible, SIGNAL(toggled(bool)), signalMapper, SLOT(map()));
    connect(signalMapper, SIGNAL(mapped(int)), dataStructure.get(), SLOT(togglePointerVisibility(int)));

    connect(pointerType.get(), SIGNAL(nameChanged(QString)), this, SLOT(updatePointerTypeButtons()));

    _pointerTypeWidgets.insert(typeIdentifier, pointerPropertyWidget);
    _pointerTypeButtons.insert(typeIdentifier, pointerTypeButton);

    return true;
}


void DataStructurePropertiesWidget::updateDataTypeButtons()
{
    QMap<int, KPushButton*>::const_iterator dataTypeWidget = _dataTypeButtons.constBegin();
    while (dataTypeWidget != _dataTypeButtons.constEnd()) {
        Document* activeDocument = DocumentManager::self()->activeDocument();
        DataTypePtr dataType = activeDocument->dataType(dataTypeWidget.key());
        dataTypeWidget.value()->setIcon(dataType->icon());
        dataTypeWidget.value()->setText(dataType->name());
        ++dataTypeWidget;
    }
}


void DataStructurePropertiesWidget::updatePointerTypeButtons()
{
    QMap<int, KPushButton*>::const_iterator pointerTypeWidget = _pointerTypeButtons.constBegin();
    while (pointerTypeWidget != _pointerTypeButtons.constEnd()) {
        Document* activeDocument = DocumentManager::self()->activeDocument();
        PointerTypePtr pointerType = activeDocument->pointerType(pointerTypeWidget.key());
        pointerTypeWidget.value()->setText(pointerType->name());
        ++pointerTypeWidget;
    }
}
