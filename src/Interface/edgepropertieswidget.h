/*
    This file is part of Rocs.
    Copyright 2009-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

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

#ifndef EDGEPROPERTIESWIDGET_H
#define EDGEPROPERTIESWIDGET_H

#include <QtGui/QWidget>
#include "ui_EdgePropertiesWidget.h"
#include <QString>
#include <QColor>
#include <QPointer>
#include "Rocs_Typedefs.h"

class Pointer;
class MainWindow;
#include <QPointF>

class PointerPropertiesWidget : public QWidget, public Ui::EdgePropertiesWidget
{
    Q_OBJECT
public:
    PointerPropertiesWidget(MainWindow *parent);
    void setPointer(PointerPtr e, QPointF pos);

public slots:
    /** shows the widget; this is a wrapper for \see setActive(true) **/
    void show() {
        setActive(true);
    };
    void on__color_activated(const QColor& c);
    void on__style_activated(int index);
    void reflectAttributes();
    void on__addProperty_clicked();
    void disconnectPointer();
    void setWidth(double v);

private:
    void setActive(bool active);

    PointerPtr _pointer;
};

#endif // EDGEPROPERTIESWIDGET_H
