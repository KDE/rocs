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

#include <QGraphicsItem>
#include <QRadioButton>


DataStructurePropertiesWidget::DataStructurePropertiesWidget(DataStructurePtr g, MainWindow* parent)
    : KButtonGroup(parent)
{
    setupUi(this);
    _mainWindow = parent;

    _dataStructure = g;
    _dataStructureName->setText(_dataStructure->name());
    _dataStructureEdgeColor->setColor(_dataStructure->pointerDefaultColor());
    _dataStructureNodeColor->setColor(_dataStructure->dataDefaultColor());
//     _dataStructureVisible->setChecked( ! _dataStructure->readOnly());
    _activateGraph->setChecked(true);
    _showEdgeNames->setChecked(_dataStructure->pointerNameVisibility());
    _showEdgeValues->setChecked(_dataStructure->pointerValueVisibility());
    _showNodeNames->setChecked(_dataStructure->dataNameVisibility());
    _showNodeValues->setChecked(_dataStructure->dataValueVisibility());

    _editWidget->setVisible(_activateGraph->isChecked());

    if (!_extraProperties->layout()) {
        QLayout * lay = DataStructurePluginManager::self()->dataStructureExtraProperties(g, _extraProperties);
        _extraProperties->setLayout(lay);
    }

    Document *gDocument = qobject_cast<Document*>(g->parent());
    connect(this, SIGNAL(addGraph(QString)), gDocument, SLOT(addDataStructure(QString)));
    connect(this, SIGNAL(removeGraph(DataStructurePtr)), g.get(), SLOT(remove()));

    connect(_dataStructureEdgeColor, SIGNAL(activated(QColor)), this, SLOT(setPointerDefaultColor(QColor)));
    connect(_dataStructureNodeColor, SIGNAL(activated(QColor)), this, SLOT(setDataDefaultColor(QColor)));

    connect(_showEdgeNames,  SIGNAL(toggled(bool)), g.get(), SLOT(setPointerNameVisibility(bool)));
    connect(_showEdgeValues, SIGNAL(toggled(bool)), g.get(), SLOT(setPointerValueVisibility(bool)));
    connect(_showNodeNames,  SIGNAL(toggled(bool)), g.get(), SLOT(setDataNameVisibility(bool)));
    connect(_showNodeValues, SIGNAL(toggled(bool)), g.get(), SLOT(setDataValueVisibility(bool)));

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

void DataStructurePropertiesWidget::on__dataStructurePointerColorApplyNow_clicked()
{
    _dataStructure->setPointersColor(_dataStructureEdgeColor->color());
}

void DataStructurePropertiesWidget::on__dataStructureDataColorApplyNow_clicked()
{
    _dataStructure->setDataColor(_dataStructureNodeColor->color());
}

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
