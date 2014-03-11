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

#ifndef SUI_DATA_STRUCTURE_PROPERTIES_DIALOG_H
#define SUI_DATA_STRUCTURE_PROPERTIES_DIALOG_H

#include "CoreTypes.h"

#include <QDialog>

class DataStructurePage;
class DataTypePage;
class PointerTypePage;

/*!
    \brief Properties widget for DataStructure.
*/
class DataStructurePropertiesDialog :
    public QDialog
{
    Q_OBJECT

public:
    explicit DataStructurePropertiesDialog(QWidget* parent = 0);

public slots:
    void setDataStructure(DataStructurePtr dataStructure);
    void setPosition(QPointF screenPosition);

private:
    DataStructurePage* _dataStructurePage;
    DataTypePage* _dataTypePage;
    PointerTypePage* _pointerTypePage;
    DataStructurePtr _dataStructure;
};

#endif
