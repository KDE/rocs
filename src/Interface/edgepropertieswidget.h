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

class Edge;

class EdgePropertiesWidget : public QWidget, public Ui::EdgePropertiesWidget
{
    Q_OBJECT
public:
    EdgePropertiesWidget();
    void setEdge(Edge *e);

public slots:
    void on__name_textChanged(const QString& s);
    void on__value_textChanged(const QString& s);
    void on__color_activated(const QColor& c);
    void on__width_valueChanged(double v);
    void on__style_activated(const QString& s);
    void reflectAttributes();

private:
    Edge *_edge;
};

#endif // EDGEPROPERTIESWIDGET_H
