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

#ifndef SUI_POINTER_TYPE_PAGE_H
#define SUI_POINTER_TYPE_PAGE_H

#include "ui_PointerTypePage.h"

#include "Rocs_Typedefs.h"

#include <QWidget>
#include <bitset>

class Document;
class PointerTypePropertyModel;

/*!
    \brief Properties page for PointerType.
*/
class PointerTypePage :
    public QWidget
{
    Q_OBJECT

public:
    explicit PointerTypePage(QWidget* parent = 0);

public slots:
    void setDocument(Document* document);
    void setPointerType(PointerTypePtr pointerType);

private slots:
    void setCurrentType(int index);
    void setTypeName();
    void setTypeDefaultColor();
    void addProperty();
    void removeProperty();
    void addType();
    void removeType();
    void updateCurrentTypeName();
    void updateCurrentTypeDirection();
    void updateCurrentTypeLineStyle();

private:
    Document* _document;
    PointerTypePropertyModel* _model;
    Ui::PointerTypePage *ui;
};

#endif
