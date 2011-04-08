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



#include <Document.h>
#include <DataStructure.h>
#include <DocumentManager.h>
#include "generategraphwidget.h"
#include "ui_generategraphwidget.h"

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

#include <QDesktopWidget>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/random/mersenne_twister.hpp>


//TODO output usefull error message
namespace boost { void throw_exception( std::exception const & ) {} } // do noop on exception

class QPushButton;

GenerateGraphWidget::GenerateGraphWidget(Document* graphDoc, QWidget* parent)
    :   QWidget(parent)
{
    ui = new Ui::GenerateGraphWidget;
    selectedGraphType_ = MESH;
	ui->setupUi(this);
    graphDoc_ = graphDoc;
    
    // put widget at center of screen
    //TODO problems with two screens
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); 
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}


void GenerateGraphWidget::setGraphType(int type)
{
    selectedGraphType_ = type;
}


void GenerateGraphWidget::generateGraph()
{
    switch(selectedGraphType_)
    {
        case MESH: {
            generateMesh(ui->meshRows->value(), ui->meshColumns->value());
            break; 
        }
        case CIRCLE: {
            generateCircle(ui->circleNodes->value()); 
            break;
        }
        case STAR: {
            generateStar(ui->starSatelliteNodes->value());
            break;
        }
        case RANDOM: {
            generateRandomGraph(
                ui->randomNodes->value(),
                ui->randomEdges->value(),
                ui->randomGeneratorSeed->value(),
                ui->randomAllowSelfedges->isTristate()
            );
            break;
        }
        default:     break;
    }
    
    close();
    deleteLater();
}

GenerateGraphWidget::~GenerateGraphWidget()
{
    delete ui;
}


void GenerateGraphWidget::generateMesh(int rows, int columns)
{
    DocumentManager::self()->activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();
    
    if (! graphDoc_ ){
      return;
    }
    if (rows<1)     rows=1;
    if (columns<1)  columns=1;

    // use active data structure iff empty
    DataStructure* graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Mesh Graph") );

    // create mesh of NxN points
    QMap<QPair<int, int>, Data*> meshNodes;

    // create mesh nodes, store them in map
    for (int i = 0; i < columns; i++) {
    for (int j = 0; j < rows; j++) {
        meshNodes[qMakePair(i,j)] = graph->addData( QString("%1-%2").arg(i).arg(j),
                                                    QPointF(i*50, j*50)-QPoint((int)25*columns,(int)25*rows)+center
                                                  );
    }
    }

    // connect mesh nodes
    for (int i = 0; i < columns; i++) {
    for (int j = 0; j < rows; j++) {
        graph->addPointer ( meshNodes[qMakePair(i,j)], meshNodes[qMakePair(i,j+1)] ); // left
        graph->addPointer ( meshNodes[qMakePair(i,j)], meshNodes[qMakePair(i+1,j)] );  // bottom.
    }
    }
}

void GenerateGraphWidget::generateStar(int numberSatelliteNodes)
{
    DocumentManager::self()->activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();
    
    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50*numberSatelliteNodes/(2*PI_);

    if ( !graphDoc_ ){
      return;
    }

    // use active data structure iff empty
    DataStructure* graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Star Graph") );

    QList< QPair<QString, QPointF> > starNodes;
    for (int i=1; i<=numberSatelliteNodes; i++) {
        starNodes << qMakePair(
            QString("%1").arg(i),
            QPointF(sin(i*2*PI_/numberSatelliteNodes)*radius, cos(i*2*PI_/numberSatelliteNodes)*radius)+center
        );
    }
    QList<Data*> nodeList = graph->addDataList(starNodes);

    // middle
    nodeList.prepend( graph->addData(QString("center"),center) );

    // connect circle nodes
    for (int i=1; i<=numberSatelliteNodes; i++) {
        graph->addPointer (nodeList.at(0),nodeList.at(i));
    }
}

void GenerateGraphWidget::generateCircle(int numberNodes)
{
    DocumentManager::self()->activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50*numberNodes/(2*PI_);

    if (! graphDoc_ ){
        return;
    }

    // use active data structure iff empty
    DataStructure* graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Circle Graph") );

    QList< QPair<QString, QPointF> > circleNodes;

    // create mesh nodes, store them in map
    for (int i=1; i<=numberNodes; i++) {
        circleNodes << qMakePair(
            QString("%1").arg(i),
            QPointF(sin(i*2*PI_/numberNodes)*radius, cos(i*2*PI_/numberNodes)*radius)+center
        );
    }
    QList<Data*> nodeList = graph->addDataList(circleNodes);

    // connect circle nodes
    for (int i=0; i<numberNodes-1; i++) {
        graph->addPointer (nodeList.at(i),nodeList.at(i+1));
    }
    graph->addPointer (nodeList.at(numberNodes-1),nodeList.at(0));
}

void GenerateGraphWidget::generateRandomGraph(int nodes, int randomEdges, int seed, bool selfEdges)
{
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();

    BoostGraph randomGraph;
    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));

    // generate graph
    boost::generate_random_graph<BoostGraph,boost::mt19937>(
        randomGraph,
        nodes,
        randomEdges,
        gen,
        selfEdges
    );

    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, center.x()-20*nodes, center.y()-20*nodes, center.x()+20*nodes, center.y()+20*nodes);
    PositionMap positionMap = boost::get(&VertexProperties::point, randomGraph);
    boost::random_graph_layout(randomGraph, positionMap, topology);

    // put generated random graph at whiteboard
    // use active data structure iff empty
    DataStructure* graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Random Graph") );

    // put nodes at whiteboard as generated
    QMap<int, Data*> mapNodes;
    int index=0;
    BGL_FORALL_VERTICES(v, randomGraph, BoostGraph) {
        randomGraph[v].index = index++;
        mapNodes[randomGraph[v].index] = graph->addData(
            QString("%1").arg(randomGraph[v].index),
            QPointF(randomGraph[v].point[0],randomGraph[v].point[1])
        );
    }
    BGL_FORALL_EDGES(e, randomGraph, BoostGraph) {
        graph->addPointer (
            mapNodes[randomGraph[boost::source<>(e, randomGraph)].index],
            mapNodes[randomGraph[boost::target<>(e, randomGraph)].index]
        );
    }
}


#include "generategraphwidget.moc"
