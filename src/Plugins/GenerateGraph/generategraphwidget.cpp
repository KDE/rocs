/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "generategraphwidget.h"
#include "ui_generategraphwidget.h"

#include "Document.h"
#include "DataStructure.h"
#include "DocumentManager.h"

#include <cmath>
#include <KLocale>
#include <KDialog>
#include <KComboBox>

#include <QtGui/QDesktopWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>


//TODO output usefull error message
namespace boost { void throw_exception( std::exception const & ) {} } // do noop on exception

class QPushButton;

GenerateGraphWidget::GenerateGraphWidget(Document* graphDoc, QWidget* parent)
    :   KDialog(parent)
{
    selectedGraphType_ = MESH;
    graphDoc_ = graphDoc;
  
    QWidget *widget = new QWidget( this );
    ui = new Ui::GenerateGraphWidget;
    ui->setupUi(this);
    setMainWidget(widget);
    
    // other KDialog options
    setCaption( i18n("Generate Graph") );
    setButtons( KDialog::Cancel | KDialog::Ok);
     
    connect( this, SIGNAL(okClicked()), this, SLOT(generateGraph()));
    
    // put widget at center of screen
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.screenNumber(parent)); 
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
        case ER_RANDOM: {
            generateErdosRenyiRandomGraph(
                ui->GNPNodes->value(),
                ui->GNPEdgeProbability->value(),
                ui->randomGeneratorSeed->value(),
                ui->GNPAllowSelfedges->isTristate()
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
    DataStructurePtr graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Mesh Graph") );

    // create mesh of NxN points
    QMap<QPair<int, int>, DataPtr > meshNodes;

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
    DataStructurePtr graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Star Graph") );

    QList< QPair<QString, QPointF> > starNodes;
    for (int i=1; i<=numberSatelliteNodes; i++) {
        starNodes << qMakePair(
            QString("%1").arg(i),
            QPointF(sin(i*2*PI_/numberSatelliteNodes)*radius, cos(i*2*PI_/numberSatelliteNodes)*radius)+center
        );
    }
    QList< DataPtr > nodeList = graph->addDataList(starNodes);

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
    DataStructurePtr graph = DocumentManager::self()->activeDocument()->activeDataStructure();
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
    QList< DataPtr > nodeList = graph->addDataList(circleNodes);

    // connect circle nodes
    for (int i=0; i<numberNodes-1; i++) {
        graph->addPointer (nodeList.at(i),nodeList.at(i+1));
    }
    graph->addPointer (nodeList.at(numberNodes-1),nodeList.at(0));
}

void GenerateGraphWidget::generateRandomGraph(int nodes, int randomEdges, int seed, bool selfEdges)
{
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();

    Graph randomGraph;
    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));

    // generate graph
    boost::generate_random_graph<Graph,boost::mt19937>(
        randomGraph,
        nodes,
        randomEdges,
        gen,
        selfEdges
    );

    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, center.x()-20*nodes, center.y()-20*nodes, center.x()+20*nodes, center.y()+20*nodes);
    PositionVec position_vec(boost::num_vertices( randomGraph ));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, randomGraph));

    boost::random_graph_layout(randomGraph, positionMap, topology);

    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
        (   randomGraph,
            positionMap,
            topology,
            boost::cooling(boost::linear_cooling<double>(100)) 
        );

    // put generated random graph at whiteboard
    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("RandomGraph") );

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index=0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = graph->addData(
                QString("%1").arg( index++ ),
                QPointF(positionMap[*vi][0],positionMap[*vi][1])
            );
    }
    
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei !=ei_end; ++ei) {
        graph->addPointer ( mapNodes[boost::source(*ei, randomGraph)],
                            mapNodes[boost::target(*ei, randomGraph)]);
            
    }
}


void GenerateGraphWidget::generateErdosRenyiRandomGraph(int nodes, double edgeProbability, int seed, bool selfEdges)
{
    QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();
    
    boost::mt19937 gen;
    gen.seed (static_cast<unsigned int>(seed));

    // generate graph
    typedef boost::erdos_renyi_iterator<boost::mt19937, Graph> ergen;
    Graph randomGraph(ergen(gen, nodes, edgeProbability, selfEdges), ergen(), nodes);
    
    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, center.x()-20*nodes, center.y()-20*nodes, center.x()+20*nodes, center.y()+20*nodes);
    PositionVec position_vec(boost::num_vertices( randomGraph ));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, randomGraph));
    boost::random_graph_layout(randomGraph, positionMap, topology);

    // put generated random graph at whiteboard
    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self()->activeDocument()->activeDataStructure();
    if (graph->dataList().size()>0)
        graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("RandomGraph") );

    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
        (   randomGraph,
            positionMap,
            topology,
            boost::cooling(boost::linear_cooling<double>(100)) 
        );

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index=0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = graph->addData(
                QString("%1").arg( index++ ),
                QPointF(positionMap[*vi][0],positionMap[*vi][1])
            );
    }
    
    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei !=ei_end; ++ei) {
        graph->addPointer ( mapNodes[boost::source(*ei, randomGraph)],
                            mapNodes[boost::target(*ei, randomGraph)]);
            
    }
}

#include "generategraphwidget.moc"
