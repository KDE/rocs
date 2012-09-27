/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
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

#ifndef POINTERPROPERTIESWIDGET_H
#define POINTERPROPERTIESWIDGET_H

#include <QtGui/QWidget>
#include "ui_PointerPropertiesWidget.h"
#include <QString>
#include <QColor>
#include <QPointer>
#include "Rocs_Typedefs.h"

class Pointer;
class MainWindow;
#include <QPointF>

class PointerPropertiesWidget : public KDialog
{
    Q_OBJECT
public:
    explicit PointerPropertiesWidget(PointerPtr pointer, QWidget *parent = 0);
    void setPointer(PointerPtr e);
    void setPosition(QPointF screenPosition);

public slots:
    void reflectAttributes();
    void setPointerType(int pointerTypeIndex);
    void updatePointerTypes();
    void updateProperties();
    void setWidth(double v);
    void setColor(const QColor& c);;

private:
    PointerPtr _pointer;
    Ui::PointerPropertiesWidget* ui;
};

#endif // POINTERPROPERTIESWIDGET_H
