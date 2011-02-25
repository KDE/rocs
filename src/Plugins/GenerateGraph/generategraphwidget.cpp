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
#include <Document.h>
#include <DataStructure.h>
#include <DocumentManager.h>

#include <cmath>

#include <KLocale>

#include <QtGui/QGridLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

#include <QtCore/QMap>
#include <QtCore/QPair>

class QPushButton;


GenerateGraphWidget::GenerateGraphWidget(Document* graphDoc, QWidget* parent)
    :   QDialog(parent),
        numberOfNodes_(10)
{
    graphDoc_ = graphDoc;

    setWindowTitle( i18n("Generate Graph") );
    setMinimumWidth( 200 );
    setMinimumHeight( 150 );

    QComboBox* comboSelectGraphType = new QComboBox();

    selectedGraphType_ = MESH;
    comboSelectGraphType->insertItem(MESH, i18n("Mesh Graph"));
    comboSelectGraphType->insertItem(CIRCLE, i18n("Circle Graph"));
    comboSelectGraphType->insertItem(STAR, i18n("Star Graph"));

    QObject::connect(comboSelectGraphType, SIGNAL(activated(int)), this, SLOT(setOptionsForGraphType(int)));

        // Buttons
    QPushButton* buttonGenerateGraph = new QPushButton(i18n("generate"));
    connect( buttonGenerateGraph, SIGNAL(clicked()), this, SLOT(generateGraph()));

        // make layout
    gridLayout_ = new QGridLayout();

        // generate options GUI
    graphOptionsWidget_ = new QWidget();
    QLabel *labelNumberNodes = new QLabel(this);
    labelNumberNodes->setText(i18n("Number of Nodes:"));
    QSpinBox *inputNumberNodes = new QSpinBox();

    connect( inputNumberNodes, SIGNAL(valueChanged(int)), this, SLOT(setNumberOfNodes(int)));
    inputNumberNodes->setValue(10);

    QGridLayout* optionsLayout = new QGridLayout();
    optionsLayout->addWidget(labelNumberNodes,0,0);
    optionsLayout->addWidget(inputNumberNodes,0,1);
    graphOptionsWidget_->setLayout(optionsLayout);

    gridLayout_->addWidget(comboSelectGraphType, 0, 0);
    gridLayout_->addWidget(graphOptionsWidget_, 1, 0);
    gridLayout_->addWidget(buttonGenerateGraph, 2, 0);

    setLayout(gridLayout_);
}


void GenerateGraphWidget::setOptionsForGraphType(int selectedGraphType)
{
    selectedGraphType_ = selectedGraphType;
}


void GenerateGraphWidget::generateGraph()
{
    switch(selectedGraphType_)
    {
        case MESH:   generateMesh();   break;
        case CIRCLE: generateCircle(); break;
        case STAR:   generateStar();   break;
        default:     break;
    }
}


void GenerateGraphWidget::generateMesh()
{
    if (! graphDoc_ ){
      return;
    }

    DataStructure* graph = DocumentManager::self()->activeDocument()->addDataStructure("Mesh Graph");
    int n = numberOfNodes_;

    // create mesh of NxN points
    QMap<QPair<int, int>, Data*> meshNodes;

    // create mesh nodes, store them in map
    for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        meshNodes[qMakePair(i,j)] = graph->addData(QString("%1-%2").arg(i).arg(j),QPointF(50+i*50, 50+j*50));
    }
    }

    // connect mesh nodes
    for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
        graph->addPointer ( meshNodes[qMakePair(i,j)], meshNodes[qMakePair(i,j+1)] ); // left
        graph->addPointer ( meshNodes[qMakePair(i,j)], meshNodes[qMakePair(i+1,j)] );  // bottom.
    }
    }

    close();
}

void GenerateGraphWidget::generateStar()
{
    int affineX = 300;
    int affineY = 300;

    if ( !graphDoc_ ){
      return;
    }
    DataStructure* graph = DocumentManager::self()->activeDocument()->addDataStructure("Star Graph");
    int n = numberOfNodes_;

    // create mesh of NxN points
    QList<Data*> starNodes;

    // create mesh nodes, store them in map
    for (int i=1; i<=n; i++) {
        starNodes << graph->addData(QString("%1").arg(i),QPointF(affineX + sin(i*2*PI_/n)*100, affineY + cos(i*2*PI_/n)*100));
    }

    // middle
    starNodes.prepend( graph->addData(QString("center"),QPointF(affineX, affineY)) );

    // connect circle nodes
    for (int i=1; i<=n; i++) {
        graph->addPointer (starNodes.at(0),starNodes.at(i));
    }

    close();
}

void GenerateGraphWidget::generateCircle()
{
    int affineX = 300;
    int affineY = 300;

    if (! graphDoc_ ){
        return;
    }

    DataStructure* graph = DocumentManager::self()->activeDocument()->addDataStructure("Ring Graph");

    int n = numberOfNodes_;

    // create mesh of NxN points
    QList<Data*> circleNodes;

    // create mesh nodes, store them in map
    for (int i=1; i<=n; i++) {
            circleNodes << graph->addData(QString("%1").arg(i),QPointF(affineX + sin(i*2*PI_/n)*100, affineY + cos(i*2*PI_/n)*100));
    }

    // connect circle nodes
    for (int i=0; i<n; i++) {
        graph->addPointer (circleNodes.at(i),circleNodes.at(i+1));
    }
    graph->addPointer (circleNodes.at(n-1),circleNodes.at(0));

    close();
}

#include "generategraphwidget.moc"
