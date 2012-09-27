/*
    This file is part of Rocs.
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

#ifndef SUI_DATAPROPERTIESWIDGET_H
#define SUI_DATAPROPERTIESWIDGET_H

#include <KDialog>
#include "Rocs_Typedefs.h"
#include "ui_DataPropertiesWidget.h"

class Data;
class MainWindow;
class DataItem;
#include <QPointF>
#include <QPointer>
/*! \brief Properties Area. */
class DataPropertiesWidget: public KDialog
{
    Q_OBJECT

public:
    explicit DataPropertiesWidget(DataPtr data, QWidget* parent = 0);
    void setPosition(QPointF screenPosition);
    void setData(DataPtr data);

private slots:
    void colorChanged(const QColor& c);;
    void reflectAttributes();
    void setDataType(int dataTypeIndex);
    void updateDataTypes();
    void updateProperties();

private:
    DataPtr _data;
    Ui::DataPropertiesWidget* ui;
    QString _oldDataStructurePlugin;
};
#endif
