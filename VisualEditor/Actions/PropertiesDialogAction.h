/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef PROPERTIESDIALOGACTION_H
#define PROPERTIESDIALOGACTION_H

#include "VisualEditorExport.h"
#include <QAction>
#include "CoreTypes.h"

class QPointF;
class Document;

class VISUALEDITOR_EXPORT PropertiesDialogAction : public QAction
{
    Q_OBJECT

    enum DialogType {
        DOCUMENT,
        DATASTRUCTURE,
        DATA,
        DATATYPE,
        POINTER,
        POINTERTYPE
    };

public:
    PropertiesDialogAction(QString text, Document* document, QObject* parent);
    PropertiesDialogAction(QString text, DataStructurePtr dataStructure, QObject* parent);
    PropertiesDialogAction(QString text, DataPtr data, QObject* parent);
    PropertiesDialogAction(QString text, PointerPtr pointer, QObject* parent);
    PropertiesDialogAction(QString text, DataTypePtr dataType, QObject* parent);
    PropertiesDialogAction(QString text, PointerTypePtr pointerType, QObject* parent);

    void setPosition(QPointF screenPosition);

public slots:
    void showDialog();

private:
    Document* _document;
    DataStructurePtr _dataStructure;
    DataPtr _data;
    PointerPtr _pointer;
    DataTypePtr _dataType;
    PointerTypePtr _pointerType;
    QPointF _screenPosition;

    DialogType _dialogType;
};


#endif // PROPERTIESDIALOGACTION_H
