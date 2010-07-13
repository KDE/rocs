/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "IncludeManagerSettings.h"
#include <QGridLayout>
#include <settings.h>



IncludeManagerSettings::IncludeManagerSettings ( QWidget* parent, Qt::WindowFlags f ) : QWidget ( parent, f ) {

//     _listModel = new QStringListModel(this);
    //_listModel->setStringList();
    QGridLayout * lay = new QGridLayout(this);
//     _listView = new QListView(this);
    kcfg_includePath = new QLineEdit(this);
    lay->addWidget(kcfg_includePath,0,1);
    this->setLayout(lay);
    readConfig();
    connect(kcfg_includePath, SIGNAL(textChanged(QString)), this, SLOT(saveSettings()));
}


IncludeManagerSettings::~IncludeManagerSettings() {

}

void IncludeManagerSettings::readConfig() {
    kcfg_includePath->setText(Settings::includePath().join(":"));
}
void IncludeManagerSettings::saveSettings() {
    Settings::setIncludePath(kcfg_includePath->text().split(':', QString::SkipEmptyParts));

}




