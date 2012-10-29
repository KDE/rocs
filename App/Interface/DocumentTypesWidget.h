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

#ifndef DOCUMENTTYPESWIDGET_H
#define DOCUMENTTYPESWIDGET_H

#include <KButtonGroup>
#include "CoreTypes.h"
#include "ui_DocumentTypesWidget.h"

class Document;
class Data;
class Pointer;
class KColorCombo;
class QToolButton;
class KPushButton;

/**
 * \class DocumentTypesWidget
 * This widget allows direct access to the data and pointer types of the currently
 * activated document.
 */
class DocumentTypesWidget : public QWidget
{
    Q_OBJECT
public:
    DocumentTypesWidget(QWidget* parent);
    ~DocumentTypesWidget();

public slots:
    void updateDocument();
    void removeDocument();

private slots:
    /** Use this SLOT to register a new data element type.
     *
     * \param identifier is the unique identifier for this data element type
     */
    void registerDataType(int identifier);

    /** Use this SLOT to remove the widget for the date element type as as specified by \param identifier.
     *
     * \param identifier is the unique identifier for this pointer type
     */
    void unregisterDataType(int identifier);

    /** Use this SLOT to register a new pointer type.
     *
     * \param identifier is the unique identifier for this pointer type
     */
    void registerPointerType(int identifier);

    /** Use this SLOT to remove the widget for the pointer type as as specified by \param identifier.
     *
     * \param identifier is the unique identifier for this pointer type
     */
    void unregisterPointerType(int identifier);

    void updateDataTypeButtons();
    void updatePointerTypeButtons();

private:
    bool createDataTypeInformationWidget(int typeIdentifier, Document* document);
    bool createPointerTypeInformationWidget(int typeIdentifier, Document* document);

    KColorCombo* _pointerTypeColor;
    QMap<int, QWidget*> _dataTypeWidgets;
    QMap<int, KPushButton*> _dataTypeButtons;
    QMap<int, KPushButton*> _pointerTypeButtons;
    QMap<int, QWidget*> _pointerTypeWidgets;
    Document* _document;

    Ui::DocumentTypesWidget *ui;
};
#endif
