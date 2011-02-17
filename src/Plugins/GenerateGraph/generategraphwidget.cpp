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


#include "generategraphwidget.h"
#include <DataType.h>
#include <Document.h>
#include <DataStructure.h>

#include <cmath>

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

class QPushButton;


GenerateGraphWidget::GenerateGraphWidget(Document* graphDoc, QWidget* parent)
    : QDialog(parent)
{
    graphDoc_ = graphDoc;


    QPushButton* buttonGenerateMesh = new QPushButton("Mesh Graph");
    connect( buttonGenerateMesh, SIGNAL(clicked()), this, SLOT(generateMesh()));

    QPushButton* buttonGenerateCircle = new QPushButton("Circle Graph");
    connect( buttonGenerateCircle, SIGNAL(clicked()), this, SLOT(generateCircle()));

    QPushButton* buttonGenerateStar = new QPushButton("Star Graph");
    connect( buttonGenerateStar, SIGNAL(clicked()), this, SLOT(generateStar()));

    //TODO connect to close of window

    // make layout
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(buttonGenerateMesh, 0, 0);
    grid->addWidget(buttonGenerateCircle, 1, 0);
    grid->addWidget(buttonGenerateStar, 2, 0);

    setLayout(grid);
}

void GenerateGraphWidget::generateMesh()
{
    Document* graphDoc = graphDoc_;
    if ( graphDoc )
    {
        DataStructure* graph = graphDoc->activeDataStructure();

        // create mesh of NxN points
        int n = 10;//TODO set by interface
        std::map< std::pair<int,int>, Data* > meshNodes;
        // create mesh nodes, store them in map
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                meshNodes[std::make_pair(i,j)] = graph->addData(
                    QString("%1-%2").arg(i).arg(j),
                    QPointF(50+i*50, 50+j*50)
                );
            }
        }
        // connect mesh nodes
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                // right edge
                if (j!=n) {
                    graph->addPointer (
                        meshNodes[std::make_pair(i,j)],
                        meshNodes[std::make_pair(i,j+1)]
                    );
                }
                //bottom edge
                if (i!=n) {
                    graph->addPointer (
                        meshNodes[std::make_pair(i,j)],
                        meshNodes[std::make_pair(i+1,j)]
                    );
                }
            }
        }
    }
}

void GenerateGraphWidget::generateStar()
{
    int affineX = 300;
    int affineY = 300;

    Document* graphDoc = graphDoc_;
    if ( graphDoc )
    {
        DataStructure * graph = graphDoc->activeDataStructure();

        // create mesh of NxN points
        int n = 10;//TODO set by interface
        std::map< int, Data* > starNodes;
        // create mesh nodes, store them in map
        for (int i=1; i<=n; i++) {
                starNodes[i] = graph->addData(
                    QString("%1").arg(i),
                    QPointF(affineX + sin(i*2*PI_/n)*100, affineY + cos(i*2*PI_/n)*100)
                );
        }
        // middle
        starNodes[0] = graph->addData(
                    QString("center"),
                    QPointF(affineX, affineY)
                );

        // connect circle nodes
        for (int i=1; i<=n; i++) {
            // bottom edge
            graph->addPointer (
                    starNodes[0],
                    starNodes[i]
                    );
        }
    }
}

void GenerateGraphWidget::generateCircle()
{
    int affineX = 300;
    int affineY = 300;

    Document* graphDoc = graphDoc_;
    if ( graphDoc )
    {
        DataStructure * graph = graphDoc->activeDataStructure();

        // create mesh of NxN points
        int n = 10;//TODO set by interface
        std::map< int, Data* > circleNodes;
        // create mesh nodes, store them in map
        for (int i=1; i<=n; i++) {
                circleNodes[i] = graph->addData(
                    QString("%1").arg(i),
                    QPointF(affineX + sin(i*2*PI_/n)*100, affineY + cos(i*2*PI_/n)*100)
                );
        }
        // connect circle nodes
        for (int i=0; i<n; i++) {
            // bottom edge
            graph->addPointer (
                    circleNodes[i],
                    circleNodes[i+1]
                    );
        }
        graph->addPointer (
                    circleNodes[n],
                    circleNodes[1]
                    );
    }
}

#include "generategraphwidget.moc"