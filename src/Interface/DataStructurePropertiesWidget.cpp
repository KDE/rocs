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

#include <KLocale>
#include <KDebug>
#include <KAction>
#include <KColorCombo>

#include <QGraphicsItem>
#include <QRadioButton>


DataStructurePropertiesWidget::DataStructurePropertiesWidget(DataStructurePtr dataStructure, MainWindow* parent)
    : KButtonGroup(parent)
{
    setupUi(this);
    _mainWindow = parent;

    // create default data element setups
    createDataTypeInformationWidget(0, dataStructure);
    createPointerTypeInformationWidget(0, dataStructure);

    _dataStructure = dataStructure;
    _dataStructureName->setText(_dataStructure->name());
//     _dataStructureVisible->setChecked( ! _dataStructure->readOnly());
    _activateGraph->setChecked(true);

    _editWidget->setVisible(_activateGraph->isChecked());

    if (!_extraProperties->layout()) {
        QLayout * lay = DataStructurePluginManager::self()->dataStructureExtraProperties(dataStructure, _extraProperties);
        _extraProperties->setLayout(lay);
    }

    Document *gDocument = qobject_cast<Document*>(dataStructure->parent());

    connect(this, SIGNAL(addGraph(QString)), gDocument, SLOT(addDataStructure(QString)));
    connect(this, SIGNAL(removeGraph(DataStructurePtr)), dataStructure.get(), SLOT(remove()));

    // react on new data types and pointer types
    connect(dataStructure.get(), SIGNAL(dataTypeCreated(int)), this, SLOT(registerDataType(int)));
    connect(dataStructure.get(), SIGNAL(dataTypeRemoved(int)), this, SLOT(unregisterDataType(int)));
    connect(dataStructure.get(), SIGNAL(pointerTypeCreated(int)), this, SLOT(registerPointerType(int)));
    connect(dataStructure.get(), SIGNAL(pointerTypeRemoved(int)), this, SLOT(unregisterPointerType(int)));

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
}


QRadioButton *DataStructurePropertiesWidget::radio()const
{
    return _activateGraph;
}


void DataStructurePropertiesWidget::on__activateGraph_toggled(bool b)
{
    _editWidget->setVisible(b);
    if (b) {
        DocumentManager::self()->activeDocument()->setActiveDataStructure(_dataStructure);
    }
}


void DataStructurePropertiesWidget::on__dataStructureDelete_clicked()
{
    emit removeGraph(_dataStructure);
}


void DataStructurePropertiesWidget::on__dataStructureName_textChanged(const QString& s)
{
    _activateGraph->setText(s);
}


bool DataStructurePropertiesWidget::createDataTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure)
{
    if(!dataStructure->dataTypeList().contains(typeIdentifier)){
        return false;
    }

    // create default data element setups
    QWidget* dataPropertyWidget = new QWidget(this);
    QGridLayout* dataPropertyLayout = new QGridLayout(dataPropertyWidget);

    QLabel* dataElementName = new QLabel("Data", dataPropertyWidget);
    _dataTypeColor = new KColorCombo(dataPropertyWidget);
    _dataTypeColor->setColor(dataStructure->dataType(typeIdentifier)->defaultColor());
    _dataTypeColor->setToolTip(i18n("Set default color for data elements"));

    KPushButton* dataTypeShowName = new KPushButton(dataPropertyWidget);
    dataTypeShowName->setIcon(KIcon("rocstexticon"));
    dataTypeShowName->setCheckable(true);
    dataTypeShowName->setChecked(true);
    dataTypeShowName->setToolTip("Show names of data elements");
    dataTypeShowName->setFixedWidth(24);

    KPushButton* dataTypeShowValue = new KPushButton(dataPropertyWidget);
    dataTypeShowValue->setIcon(KIcon("rocsvalueicon"));
    dataTypeShowValue->setCheckable(true);
    dataTypeShowValue->setChecked(true);
    dataTypeShowValue->setToolTip(i18n("Show values of data elements"));
    dataTypeShowValue->setFixedWidth(24);

//     KPushButton* dataTypeDisplay = new KPushButton(this);
//     dataTypeDisplay->setIcon(KIcon("rocseyeblack"));
//     dataTypeDisplay->setCheckable(true);
//     dataTypeDisplay->setChecked(true);
//     dataTypeDisplay->setFixedWidth(24);

    dataPropertyWidget->setLayout(dataPropertyLayout);
    dataPropertyLayout->addWidget(dataElementName, 1, 1);
    dataPropertyLayout->addWidget(_dataTypeColor, 1, 2);
    dataPropertyLayout->addWidget(dataTypeShowName, 1, 3);
    dataPropertyLayout->addWidget(dataTypeShowValue, 1, 4);
//     _dataTypeProperties->addWidget(dataTypeDisplay,1,5);

    _typeProperties->addWidget(dataPropertyWidget);

    connect(dataTypeShowName, SIGNAL(toggled(bool)),
        dataStructure->dataType(typeIdentifier).get(), SLOT(setNameVisible(bool)));
    connect(dataTypeShowValue, SIGNAL(toggled(bool)),
        dataStructure->dataType(typeIdentifier).get(), SLOT(setValueVisible(bool)));
    connect(_dataTypeColor, SIGNAL(activated(QColor)),
        dataStructure->dataType(typeIdentifier).get(), SLOT(setDefaultColor(QColor)));

    _dataTypeWidgets.insert(typeIdentifier, dataPropertyWidget);

    return true;
}


bool DataStructurePropertiesWidget::createPointerTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure)
{
    if(!dataStructure->pointerTypeList().contains(typeIdentifier)){
        return false;
    }

    // create default data element setups
    QWidget* pointerPropertyWidget = new QWidget(this);
    QGridLayout* pointerPropertyLayout = new QGridLayout(pointerPropertyWidget);

    QLabel* pointerElementName = new QLabel(i18n("Pointer"), pointerPropertyWidget);
    _pointerTypeColor = new KColorCombo(pointerPropertyWidget);
    _pointerTypeColor->setColor(dataStructure->pointerType(typeIdentifier)->defaultColor());
    _pointerTypeColor->setToolTip(i18n("Set default color for pointers"));

    KPushButton* pointerTypeShowName = new KPushButton(pointerPropertyWidget);
    pointerTypeShowName->setIcon(KIcon("rocstexticon"));
    pointerTypeShowName->setCheckable(true);
    pointerTypeShowName->setChecked(true);
    pointerTypeShowName->setToolTip(i18n("Show names of pointers"));
    pointerTypeShowName->setFixedWidth(24);

    KPushButton* pointerTypeShowValue = new KPushButton(pointerPropertyWidget);
    pointerTypeShowValue->setIcon(KIcon("rocsvalueicon"));
    pointerTypeShowValue->setCheckable(true);
    pointerTypeShowValue->setChecked(true);
    pointerTypeShowValue->setToolTip(i18n("Show values of pointers"));
    pointerTypeShowValue->setFixedWidth(24);

//     KPushButton* pointerTypeDisplay = new KPushButton(this);
//     pointerTypeDisplay->setIcon(KIcon("rocseyeblack"));
//     pointerTypeDisplay->setCheckable(true);
//     pointerTypeDisplay->setChecked(true);
//     pointerTypeDisplay->setFixedWidth(24);

    pointerPropertyWidget->setLayout(pointerPropertyLayout);
    pointerPropertyLayout->addWidget(pointerElementName, 1, 1);
    pointerPropertyLayout->addWidget(_pointerTypeColor, 1, 2);
    pointerPropertyLayout->addWidget(pointerTypeShowName, 1, 3);
    pointerPropertyLayout->addWidget(pointerTypeShowValue, 1, 4);
// //     _dataTypeProperties->addWidget(pointerTypeDisplay,2,5);

    //TODO add to specific widget for pointers
    _typeProperties->addWidget(pointerPropertyWidget);

    connect(pointerTypeShowName,  SIGNAL(toggled(bool)),
        dataStructure->pointerType(typeIdentifier).get(), SLOT(setNameVisible(bool)));
    connect(pointerTypeShowValue, SIGNAL(toggled(bool)),
        dataStructure->pointerType(typeIdentifier).get(), SLOT(setValueVisible(bool)));
    connect(_pointerTypeColor, SIGNAL(activated(QColor)),
        dataStructure->pointerType(typeIdentifier).get(), SLOT(setDefaultColor(QColor)));

    _pointerTypeWidgets.insert(typeIdentifier, pointerPropertyWidget);

    return true;
}
