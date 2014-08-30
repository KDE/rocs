/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "configuredefaultproperties.h"
#include <settings.h>

#include "ui_configuredefaultproperties.h"

ConfigureDefaultProperties::ConfigureDefaultProperties(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);

    readConfig();
    ui->kcfg_dataNodeDisplay->setCurrentIndex(_displayPositionNode);
    ui->kcfg_dataEdgeDisplay->setCurrentIndex(_displayPositionEdge);
    ui->kcfg_executionModeDebugVisible->setChecked(_executionModeDebugVisible);
    ui->kcfg_executionModeOneStepVisible->setChecked(_executionModeOneStepVisible);
}


ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}


void ConfigureDefaultProperties::readConfig()
{
    _displayPositionNode = Settings::dataNodeDisplay();
    _displayPositionEdge = Settings::dataEdgeDisplay();
    _executionModeDebugVisible = Settings::executionModeDebugVisible();
    _executionModeOneStepVisible = Settings::executionModeOneStepVisible();
}


void ConfigureDefaultProperties::saveConfig()
{
    Settings::setDataNodeDisplay(_displayPositionNode);
    Settings::setDataEdgeDisplay(_displayPositionEdge);
    Settings::setExecutionModeDebugVisible(_executionModeDebugVisible);
    Settings::setExecutionModeOneStepVisible(_executionModeOneStepVisible);

    emit showExecuteModeDebugChanged(_executionModeDebugVisible);
    emit showExecuteModeOneStepChanged(_executionModeOneStepVisible);
}


void ConfigureDefaultProperties::setDisplayPositionNode(int position)
{
    if (position < 0) {
        return;
    }
    _displayPositionNode = position;
    emit changed(true);
}


void ConfigureDefaultProperties::setDisplayPositionEdge(int position)
{
    if (position < 0) {
        return;
    }
    _displayPositionEdge = position;
    emit changed(true);
}


void ConfigureDefaultProperties::setExecutionModeDebugVisible(bool visible)
{
    _executionModeDebugVisible = visible;
    emit changed(true);
}


void ConfigureDefaultProperties::setExecutionModeOneStepVisible(bool visible)
{
    _executionModeOneStepVisible = visible;
    emit changed(true);
}
