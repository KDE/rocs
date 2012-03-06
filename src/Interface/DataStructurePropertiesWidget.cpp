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
#include "Pointer.h"
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


DataStructurePropertiesWidget::DataStructurePropertiesWidget(DataStructurePtr g, MainWindow* parent)
    : KButtonGroup(parent)
{
    setupUi(this);
    _mainWindow = parent;

    // create default data element setups
    QLabel* dataElementName = new QLabel("Data", this);
    _dataTypeColor = new KColorCombo(this);
    _dataTypeColor->setColor(g->dataDefaultColor());
    _dataTypeColor->setToolTip(i18n("Set default color for data elements"));

    KPushButton* dataTypeShowName = new KPushButton(this);
    dataTypeShowName->setIcon(KIcon("rocstexticon"));
    dataTypeShowName->setCheckable(true);
    dataTypeShowName->setChecked(true);
    dataTypeShowName->setToolTip("Show names of data elements");
    dataTypeShowName->setFixedWidth(24);

    KPushButton* dataTypeShowValue = new KPushButton(this);
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

    _dataTypeProperties->addWidget(dataElementName,1,1);
    _dataTypeProperties->addWidget(_dataTypeColor,1,2);
    _dataTypeProperties->addWidget(dataTypeShowName,1,3);
    _dataTypeProperties->addWidget(dataTypeShowValue,1,4);
//     _dataTypeProperties->addWidget(dataTypeDisplay,1,5);

    // create default data element setups
    QLabel* pointerElementName = new QLabel(i18n("Pointer"), this);
    _pointerTypeColor = new KColorCombo(this);
    _pointerTypeColor->setColor(g->pointerDefaultColor());
    _pointerTypeColor->setToolTip(i18n("Set default color for pointers"));

    KPushButton* pointerTypeShowName = new KPushButton(this);
    pointerTypeShowName->setIcon(KIcon("rocstexticon"));
    pointerTypeShowName->setCheckable(true);
    pointerTypeShowName->setChecked(true);
    pointerTypeShowName->setToolTip(i18n("Show names of pointers"));
    pointerTypeShowName->setFixedWidth(24);

    KPushButton* pointerTypeShowValue = new KPushButton(this);
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

    _dataTypeProperties->addWidget(pointerElementName,2,1);
    _dataTypeProperties->addWidget(_pointerTypeColor,2,2);
    _dataTypeProperties->addWidget(pointerTypeShowName,2,3);
    _dataTypeProperties->addWidget(pointerTypeShowValue,2,4);
//     _dataTypeProperties->addWidget(pointerTypeDisplay,2,5);

    _dataStructure = g;
    _dataStructureName->setText(_dataStructure->name());
//     _dataStructureVisible->setChecked( ! _dataStructure->readOnly());
    _activateGraph->setChecked(true);

    _editWidget->setVisible(_activateGraph->isChecked());

    if (!_extraProperties->layout()) {
        QLayout * lay = DataStructurePluginManager::self()->dataStructureExtraProperties(g, _extraProperties);
        _extraProperties->setLayout(lay);
    }

    Document *gDocument = qobject_cast<Document*>(g->parent());
    connect(dataTypeShowName, SIGNAL(toggled(bool)),g.get(), SLOT(setDataNameVisibility(bool)));
    connect(dataTypeShowValue, SIGNAL(toggled(bool)),g.get(), SLOT(setDataValueVisibility(bool)));
    connect(_dataTypeColor, SIGNAL(activated(QColor)), this, SLOT(setPointerDefaultColor(QColor)));

    connect(pointerTypeShowName,  SIGNAL(toggled(bool)), g.get(), SLOT(setPointerNameVisibility(bool)));
    connect(pointerTypeShowValue, SIGNAL(toggled(bool)), g.get(), SLOT(setPointerValueVisibility(bool)));
    connect(_pointerTypeColor, SIGNAL(activated(QColor)), this, SLOT(setDataDefaultColor(QColor)));


    connect(this, SIGNAL(addGraph(QString)), gDocument, SLOT(addDataStructure(QString)));
    connect(this, SIGNAL(removeGraph(DataStructurePtr)), g.get(), SLOT(remove()));

    connect(_dataStructureName,      SIGNAL(textChanged(QString)), g.get(), SLOT(setName(QString)));
}

DataStructurePropertiesWidget::~DataStructurePropertiesWidget()
{
}

void DataStructurePropertiesWidget::setPointerDefaultColor(QColor c)
{
    _dataStructure->setPointerDefaultColor(c);
}

void DataStructurePropertiesWidget::setDataDefaultColor(QColor c)
{
    _dataStructure->setDataDefaultColor(c);
}

// void DataStructurePropertiesWidget::on__dataStructurePointerColorApplyNow_clicked()
// {
//     _dataStructure->setPointersColor(_pointerTypeColor->color());
// }

// void DataStructurePropertiesWidget::on__dataStructureDataColorApplyNow_clicked()
// {
//     _dataStructure->setDataColor(_dataTypeColor->color());
// }

void DataStructurePropertiesWidget::on__dataStructureVisible_toggled(bool b)
{
    _dataStructure->setReadOnly(!b);
    qDebug() << "toggle visibility to " << b;
    _mainWindow->scene()->hideGraph(_dataStructure, b);
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
