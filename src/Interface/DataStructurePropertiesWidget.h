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

#ifndef SUI_DATASTRUCTUREPROPERTIESWIDGET_H
#define SUI_DATASTRUCTUREPROPERTIESWIDGET_H

#include <KButtonGroup>
#include "Rocs_Typedefs.h"
#include "ui_DataStructurePropertiesWidget.h"

class DataStructure;
class Data;
class Pointer;
class MainWindow;
class KColorCombo;
class QToolButton;
class KPushButton;

/*! \brief Properties Area, not used yet. */

class DataStructurePropertiesWidget
    : public KButtonGroup, public Ui::DataStructurePropertiesWidget
{
    Q_OBJECT
public:
    explicit DataStructurePropertiesWidget(DataStructurePtr g, MainWindow* parent = 0);
    ~DataStructurePropertiesWidget();
    QRadioButton *radio() const;

private slots:
    /** Use this SLOT to register a new data element type to be displayed at the data structure
     * widget.
     * \param identifier is the unique identifier for this data element type
     */
    void registerDataType(int identifier);

    /** Use this SLOT to remove the widget for the date element type as as specified by \param identifier
     * from the data structure widget.
     *
     * \param identifier is the unique identifier for this pointer type
     */
    void unregisterDataType(int identifier);

    /** Use this SLOT to register a new pointer type to be displayed at the data structure widget.
     * \param identifier is the unique identifier for this pointer type
     */
    void registerPointerType(int identifier);

    /** Use this SLOT to remove the widget for the pointer type as as specified by \param identifier
     * from the data structure widget.
     * \param identifier is the unique identifier for this pointer type
     */
    void unregisterPointerType(int identifier);

    void updateDataTypeButtons();
    void updatePointerTypeButtons();

    void on__dataStructureDelete_clicked();
    void on__activateGraph_toggled(bool b);

signals:
    void updateNeeded();
    void addGraph(QString name);
    void removeGraph(DataStructurePtr);

private:
    bool createDataTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure);
    bool createPointerTypeInformationWidget(int typeIdentifier, DataStructurePtr dataStructure);

    DataStructurePtr _dataStructure;
    KColorCombo* _pointerTypeColor;
    MainWindow *_mainWindow;
    QMap<int, QWidget*> _dataTypeWidgets;
    QMap<int, KPushButton*> _dataTypeButtons;
    QMap<int, KPushButton*> _pointerTypeButtons;
    QMap<int, QWidget*> _pointerTypeWidgets;
};
#endif
