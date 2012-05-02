/*
    This file is part of Rocs.
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

#include "DataStructurePropertiesFullWidget.h"

#include "DataStructurePage.h"
#include "DataTypePage.h"
#include "PointerTypePage.h"
#include <Data.h>
#include <DataItem.h>

#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>

#include <KDebug>
#include <KLineEdit>
#include <KColorCombo>
#include <KPushButton>
#include <KComboBox>
#include <KDialog>
#include <KTabWidget>
#include <QLayout>
#include <QLabel>

DataStructurePropertiesFullWidget::DataStructurePropertiesFullWidget(QWidget* parent)
    : KDialog(parent)
{
    KTabWidget *tabWidget = new KTabWidget(this);
    _dataStructurePage = new DataStructurePage(this);
    _dataTypePage = new DataTypePage(this);
    _pointerTypePage = new PointerTypePage(this);

    tabWidget->addTab(_dataStructurePage, i18n("Data Structure"));
    tabWidget->addTab(_dataTypePage, i18n("Data Types"));
    tabWidget->addTab(_pointerTypePage, i18n("Pointer Types"));

    setMainWidget(tabWidget);

    setCaption(i18n("Data Structure Properties"));
    setButtons(Close);
    setAttribute(Qt::WA_DeleteOnClose);
}


void DataStructurePropertiesFullWidget::setDataStructure(DataStructurePtr dataStructure)
{
    Q_ASSERT(dataStructure);
    _dataStructurePage->setDataStructure(dataStructure);
    _dataTypePage->setDocument(dataStructure->document());
    _pointerTypePage->setDocument(dataStructure->document());
}


void DataStructurePropertiesFullWidget::setPosition(QPointF screenPosition)
{
    move(screenPosition.x() + 10,  screenPosition.y() + 10);
}
