/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "ConfigureDefaultProperties.h"
#include "ui_ConfigureDefaultProperties.h"
#include <settings.h>
#include "GraphVisualEditor.h"
#include "GraphicsLayout.h"


ConfigureDefaultProperties::ConfigureDefaultProperties(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);

    readConfig();
    ui->nodeInformationComboBox->setCurrentIndex(_displayPositionNode);
    ui->edgeInformationComboBox->setCurrentIndex(_displayPositionEdge);
    ui->showDebugExecutionCheckBox->setChecked(_excutionModeDebugVisible);
    ui->showOneStepExecutionCheckBox->setChecked(_excutionModeOneStepVisible);
}


ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}


void ConfigureDefaultProperties::readConfig()
{
    _displayPositionNode = Settings::dataNodeDisplay();
    _displayPositionEdge = Settings::dataEdgeDisplay();
    _excutionModeDebugVisible = Settings::excutionModeDebugVisible();
    _excutionModeOneStepVisible = Settings::excutionModeOneStepVisible();
}


void ConfigureDefaultProperties::saveConfig()
{
    Settings::setDataNodeDisplay(_displayPositionNode);
    Settings::setDataEdgeDisplay(_displayPositionEdge);
    Settings::setExcutionModeDebugVisible(_excutionModeDebugVisible);
    Settings::setExcutionModeOneStepVisible(_excutionModeOneStepVisible);

    GraphicsLayout::self()->setViewStyleDataNode(_displayPositionNode);
    GraphicsLayout::self()->setViewStyleDataEdge(_displayPositionEdge);

    emit showExecuteModeDebugChanged(_excutionModeDebugVisible);
    emit showExecuteModeOneStepChanged(_excutionModeOneStepVisible);
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
    _excutionModeDebugVisible = visible;
    emit changed(true);
}


void ConfigureDefaultProperties::setExecutionModeOneStepVisible(bool visible)
{
    _excutionModeOneStepVisible = visible;
    emit changed(true);
}
