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


#ifndef TRANSFORMEDGESWIDGET_H
#define TRANSFORMEDGESWIDGET_H

#include <QWidget>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>


class QGridLayout;
class Document;
class DataStructure;

namespace Ui {
    class TransformEdgesWidget;
}

class TransformEdgesWidget :
    public QWidget
{
    Q_OBJECT


    public:
        TransformEdgesWidget(Document* graphDoc, QWidget *parent=0);
        ~TransformEdgesWidget();

        void addDataStructures(QStringList dsNames);

    public slots:
        void executeTransform();

    private:
        void makeComplete( DataStructure* graph );
        void removeAllEdges();
        void reverseAllEdges();

        Document* graphDoc_;
        static const double PI_ = 3.14159265358979323846;
        int selectedGraphType_;
        QGridLayout *gridLayout_;
        QWidget* graphOptionsWidget_;
        Ui::TransformEdgesWidget *ui;
        
};

#endif // TRANSFORMEDGESWIDGET_H
