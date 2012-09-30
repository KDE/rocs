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

#include "DataStructurePage.h"
#include <Data.h>
#include "Scene/DataItem.h"

#include <QWidget>
#include <QString>
#include <QSvgRenderer>
#include <QPainter>

#include <KDebug>
#include <KLineEdit>
#include <KColorCombo>
#include <KPushButton>
#include <KComboBox>
#include "DataStructurePluginManager.h"
#include <DataStructurePluginInterface.h>

DataStructurePage::DataStructurePage(QWidget* parent)
    : QWidget(parent)
{
    ui = new Ui::DataStructurePage;
    ui->setupUi(this);
}


void DataStructurePage::setDataStructure(DataStructurePtr dataStructure)
{
    Q_ASSERT(dataStructure);

    if (_dataStructure) {
        disconnect(_dataStructure.get());
    }

    ui->dataStructureName->setText(dataStructure->name());
    ui->dataStructurePlugin->addItem(dataStructure->document()->dataStructurePlugin()->name());
    ui->dataStructurePlugin->setDisabled(true);

    if (!ui->pluginExtraProperties->layout()) {
        QLayout * layout = DataStructurePluginManager::self()->dataStructureExtraProperties(dataStructure, ui->pluginExtraProperties);
        ui->pluginExtraProperties->setLayout(layout);
    }

    connect(ui->dataStructureName, SIGNAL(textChanged(QString)),
            dataStructure.get(), SLOT(setName(QString)));
}


