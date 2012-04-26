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

#ifndef SUI_DATA_STRUCTURE_PROPERTIES_FULL_WIDGET_H
#define SUI_DATA_STRUCTURE_PROPERTIES_FULL_WIDGET_H

#include "ui_DataStructurePropertiesFullWidget.h"
#include "Rocs_Typedefs.h"

#include <QWidget>


/*!
    \brief Properties widget for KDialog.
*/
class DataStructurePropertiesFullWidget :
    public KDialog
{
    Q_OBJECT

public:
    explicit DataStructurePropertiesFullWidget(QWidget* parent = 0);

public slots:
    void setDataStructure(DataStructurePtr dataStructure);
    void setPosition(QPointF screenPosition);

private slots:
    void addDataType();
    void removeDataType(int dataType);

private:
    void setupDataTypes();

    /**
     * creates and adds to list
     */
    QWidget* createDataTypeWidget(int dataType);

    QMap<int,QWidget*> _dataTypeWigets;
    DataStructurePtr _dataStructure;
    Ui::DataStructurePropertiesFullWidget *ui;
};

#endif
