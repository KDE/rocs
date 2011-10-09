/*
    This file is part of Rocs.
    Copyright (C) 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include "Interface/GraphVisualEditor.h"
#include "GraphicsLayout.h"


ConfigureDefaultProperties::ConfigureDefaultProperties( QWidget* parent ) :
    QWidget(parent),
    ui(new Ui::ConfigureDefaultProperties)
{
    ui->setupUi(this);

    readConfig();
    ui->comboDefaultNode->setCurrentIndex(_displayPositionNode);
    ui->comboDefaultEdge->setCurrentIndex(_displayPositionEdge);
}

ConfigureDefaultProperties::~ConfigureDefaultProperties()
{
    delete ui;
}

void ConfigureDefaultProperties::readConfig() {
    _displayPositionNode = Settings::dataNodeDisplay();
    _displayPositionEdge = Settings::dataEdgeDisplay();
}

void ConfigureDefaultProperties::saveConfig() {
    Settings::setDataNodeDisplay(_displayPositionNode);
    Settings::setDataEdgeDisplay(_displayPositionEdge);

    GraphicsLayout::self()->setViewStyleDataNode(_displayPositionNode);
    GraphicsLayout::self()->setViewStyleDataEdge(_displayPositionEdge);
}

void ConfigureDefaultProperties::setDisplayPositionNode(int position) {
    if (position<0) return;
    _displayPositionNode = position;
    emit changed(true);
}

void ConfigureDefaultProperties::setDisplayPositionEdge(int position) {
    if (position<0) return;
    _displayPositionEdge = position;
    emit changed(true);
}
