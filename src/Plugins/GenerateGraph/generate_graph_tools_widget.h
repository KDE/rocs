/*
    Rocs-Tools-Plugin: Generate graphs by specific patterns
    Copyright (C) 2011  Andreas Cord-Landwehr

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


#ifndef GENERATEGRAPHWIDGET_H
#define GENERATEGRAPHWIDGET_H

#include <DataType.h>
#include <QDialog>
#include <QString>

class GenerateGraphWidget :
    public QDialog
{
    Q_OBJECT

    public:
        GenerateGraphWidget(DataTypeDocument* graphDoc, QWidget *parent = 0);
//         ~GenerateGraphWidget();

    public slots:
        void generateMesh();
        void generateStar();
        void generateCircle();

    private:
        DataTypeDocument* graphDoc_;
        static const double PI_ = 3.14159265358979323846;
};

#endif // GENERATEGRAPHWIDGET_H

