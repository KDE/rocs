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

#include <QDialog>

class QGridLayout;
class QWidget;
class Document;

class GenerateGraphWidget :
    public QDialog
{
    Q_OBJECT

        // used for graph selection
    enum GraphType {
        MESH,
        CIRCLE,
        STAR,
        RANDOM
    };

    public:
        GenerateGraphWidget(Document* graphDoc, QWidget *parent = 0);
//         ~GenerateGraphWidget();

    public slots:
        void generateGraph();


    private slots:
        /**
         * set GUI options according to selected graph type at combo box
         * \return  void
         */
        void setOptionsForGraphType(int selectedGraphType);
        void setNumberOfNodes(int number) { numberOfNodes_ = number; };


    private:
        void generateStar();
        void generateCircle();
        void generateMesh();

        Document* graphDoc_;
        static const double PI_ = 3.14159265358979323846;
        int selectedGraphType_;
        QGridLayout *gridLayout_;
        QWidget* graphOptionsWidget_;
        int numberOfNodes_;
};

#endif // GENERATEGRAPHWIDGET_H

