/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

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

class Pointer;
class MainWindow;
#include <QPointF>

class PointerPropertiesWidget : public QWidget, public Ui::EdgePropertiesWidget
{
    Q_OBJECT
public:
    PointerPropertiesWidget(MainWindow *parent);
    void setPointer(Pointer *e, QPointF pos);

public slots:
    void on__color_activated(const QColor& c);
    void on__style_activated(int index);
    void reflectAttributes();
    void on__addProperty_clicked();
    void disconnectPointer();
    void setWidth(double v);
    
private:
    QPointer<Pointer> _pointer;
};

#endif // EDGEPROPERTIESWIDGET_H
