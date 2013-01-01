/*
    This file is part of Rocs.
    Copyright (C) 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "GenerateGraphWidget.h"
#include "ui_GenerateGraphWidget.h"

#include "Document.h"
#include "DataStructure.h"
#include "DocumentManager.h"
#include "PointerType.h"
#include "Modifiers/Topology.h"
#include "Data.h"
#include "Pointer.h"

#include <cmath>
#include <KLocale>
#include <KDialog>
#include <KComboBox>

#include <QList>
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

// typedefs used for the boost graph library
typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS,
        boost::property<boost::vertex_name_t, std::string> >
        Graph;

typedef boost::rectangle_topology<> topology_type;

typedef topology_type::point_type point_type;

typedef std::vector<point_type> PositionVec;

typedef boost::iterator_property_map < PositionVec::iterator,
        boost::property_map<Graph, boost::vertex_index_t>::type >
        PositionMap;


GenerateGraphWidget::GenerateGraphWidget(Document *graphDoc, QWidget *parent)
:   KDialog(parent)
{
    defaultIdentifiers << QString("MeshGraph") << QString("StarGraph");
    defaultIdentifiers << QString("CircleGraph") << QString("RandomGraph");
    defaultIdentifiers << QString("RandomGraph") << QString("RandomTreeGraph");

    selectedGraphType_ = MESH;
    graphDoc_ = graphDoc;

    QWidget *widget = new QWidget(this);
    ui = new Ui::GenerateGraphWidget;
    ui->setupUi(widget);
    setMainWidget(widget);

    // other KDialog options
    setCaption(i18nc("@title:window", "Generate Graph"));
    setButtons(KDialog::Cancel | KDialog::Ok);
    ui->buttonShowAdvanced->setIcon(KIcon("rocsadvancedsetup"));
    KDialog::centerOnScreen(widget, -3);

    connect(this, SIGNAL(okClicked()), this, SLOT(generateGraph()));
    connect(ui->comboGraphGenerator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGraphType(int)));

    // set random seeds
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    uint badRandomSeed = qHash(currentTime) % 99999;
    badRandomSeed = (badRandomSeed == 0) ? 1 : badRandomSeed;
    ui->randomGeneratorSeed->setValue(badRandomSeed);
    ui->GNPGeneratorSeed->setValue(badRandomSeed);
    ui->randomTreeGeneratorSeed->setValue(badRandomSeed);

    // set visibility for advanced options
    // TODO move to containers for easier handling
    ui->label_randomGeneratorSeed->setVisible(false);
    ui->randomGeneratorSeed->setVisible(false);
    ui->label_GNPGeneratorSeed->setVisible(false);
    ui->GNPGeneratorSeed->setVisible(false);
    ui->label_randomTreeGeneratorSeed->setVisible(false);
    ui->randomTreeGeneratorSeed->setVisible(false);

    Document *document = DocumentManager::self().activeDocument();

    foreach (int pointerTypeID, document->pointerTypeList()) {
        PointerTypePtr pointerType = document->pointerType(pointerTypeID);
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            pointerType->name(),
            pointerType->identifier());
        ui->pointerTypeSelector->addItem(item, QVariant(pointerTypeID));
    }
    ui->pointerTypeSelector->setCurrentIndex(0);

    foreach (int dataTypeID, document->dataTypeList()) {
        DataTypePtr dataType = document->dataType(dataTypeID);
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            dataType->name(),
            dataType->identifier());
        ui->dataTypeSelector->addItem(item, QVariant(dataTypeID));
    }
    ui->dataTypeSelector->setCurrentIndex(0);
}


void GenerateGraphWidget::setGraphType(int type)
{
    int previousGraphType_ = selectedGraphType_;
    QString text = ui->identifier->text();
    selectedGraphType_ = type;
    if (text == defaultIdentifiers.at(previousGraphType_)) {
        ui->identifier->setText(defaultIdentifiers.at(selectedGraphType_));
    }
}


void GenerateGraphWidget::generateGraph()
{
    int pointerType = ui->pointerTypeSelector->currentIndex();
    int dataType = ui->dataTypeSelector->currentIndex();
    QString identifier = ui->identifier->text();
    switch (selectedGraphType_) {
    case MESH: {
        generateMesh(ui->meshRows->value(), ui->meshColumns->value(), pointerType, dataType, identifier);
        break;
    }
    case CIRCLE: {
        generateCircle(ui->circleNodes->value(), pointerType, dataType, identifier);
        break;
    }
    case STAR: {
        generateStar(ui->starSatelliteNodes->value(), pointerType, dataType, identifier);
        break;
    }
    case RANDOM: {
        generateRandomGraph(
            ui->randomNodes->value(),
            ui->randomEdges->value(),
            ui->randomGeneratorSeed->value(),
            ui->randomAllowSelfedges->isTristate(),
            pointerType,
            dataType,
            identifier
        );
        break;
    }
    case ER_RANDOM: {
        generateErdosRenyiRandomGraph(
            ui->GNPNodes->value(),
            ui->GNPEdgeProbability->value(),
            ui->randomGeneratorSeed->value(),
            ui->GNPAllowSelfedges->isTristate(),
            pointerType,
            dataType,
            identifier
        );
        break;
    }
    case RANDOM_TREE: {
        generateRandomTreeGraph(
            ui->randomTreeNodes->value(),
            ui->randomTreeGeneratorSeed->value(),
            pointerType,
            dataType,
            identifier
        );
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

void GenerateGraphWidget::generateMesh(int rows, int columns, int pointerType, int dataType, const QString &identifier)
{
    DocumentManager::self().activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self().activeDocument()->activeDataStructure()->relativeCenter();

    if (! graphDoc_) {
        return;
    }
    if (rows < 1) {
        rows = 1;
    }
    if (columns < 1) {
        columns = 1;
    }

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    // create mesh of NxN points
    QMap<QPair<int, int>, DataPtr > meshNodes;

    // create mesh nodes, store them in map
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            meshNodes[qMakePair(i, j)] = graph->addData(QString("%1-%2").arg(i).arg(j),
                                                        QPointF(i * 50, j * 50) - QPoint((int)25 * columns, (int)25 * rows) + center,
                                                        dataType
                                                       );
        }
    }

    // connect mesh nodes
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            graph->addPointer(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i, j + 1)], pointerType); // left
            graph->addPointer(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i + 1, j)], pointerType); // bottom.
        }
    }
}

void GenerateGraphWidget::generateStar(int numberSatelliteNodes, int pointerType, int dataType, const QString &identifier)
{
    DocumentManager::self().activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self().activeDocument()->activeDataStructure()->relativeCenter();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * numberSatelliteNodes / (2 * PI_);

    if (!graphDoc_) {
        return;
    }

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    QList< QPair<QString, QPointF> > starNodes;
    for (int i = 1; i <= numberSatelliteNodes; i++) {
        starNodes << qMakePair(
                      QString("%1").arg(i),
                      QPointF(sin(i * 2 * PI_ / numberSatelliteNodes)*radius, cos(i * 2 * PI_ / numberSatelliteNodes)*radius) + center
                  );
    }
    QList< DataPtr > nodeList = graph->addDataList(starNodes, dataType);

    // middle
    nodeList.prepend(graph->addData(QString("center"), center, dataType));

    // connect circle nodes
    for (int i = 1; i <= numberSatelliteNodes; i++) {
        graph->addPointer(nodeList.at(0), nodeList.at(i), pointerType);
    }
}

void GenerateGraphWidget::generateCircle(int numberNodes, int pointerType, int dataType, const QString &identifier)
{
    DocumentManager::self().activeDocument()->activeDataStructure()->updateRelativeCenter();
    QPointF center = DocumentManager::self().activeDocument()->activeDataStructure()->relativeCenter();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * numberNodes / (2 * PI_);

    if (! graphDoc_) {
        return;
    }

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    QList< QPair<QString, QPointF> > circleNodes;

    // create mesh nodes, store them in map
    for (int i = 1; i <= numberNodes; i++) {
        circleNodes << qMakePair(
                        QString("%1").arg(i),
                        QPointF(sin(i * 2 * PI_ / numberNodes)*radius, cos(i * 2 * PI_ / numberNodes)*radius) + center
                    );
    }
    QList< DataPtr > nodeList = graph->addDataList(circleNodes, dataType);

    // connect circle nodes
    for (int i = 0; i < numberNodes - 1; i++) {
        graph->addPointer(nodeList.at(i), nodeList.at(i + 1), pointerType);
    }
    graph->addPointer(nodeList.at(numberNodes - 1), nodeList.at(0), pointerType);
}

void GenerateGraphWidget::generateRandomGraph(int nodes, int randomEdges, int seed, bool selfEdges, int pointerType, int dataType, const QString &identifier)
{
    QPointF center = DocumentManager::self().activeDocument()->activeDataStructure()->relativeCenter();

    Graph randomGraph;
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    // generate graph
    boost::generate_random_graph<Graph, boost::mt19937>(
        randomGraph,
        nodes,
        randomEdges,
        gen,
        selfEdges
    );

    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, center.x() - 20 * nodes, center.y() - 20 * nodes, center.x() + 20 * nodes, center.y() + 20 * nodes);
    PositionVec position_vec(boost::num_vertices(randomGraph));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, randomGraph));

    boost::random_graph_layout(randomGraph, positionMap, topology);

    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
    (randomGraph,
     positionMap,
     topology,
     boost::cooling(boost::linear_cooling<double>(100))
    );

    // put generated random graph at whiteboard
    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index = 0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = graph->addData(
                            QString("%1").arg(index++),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1]),
                            dataType
                        );
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        graph->addPointer(mapNodes[boost::source(*ei, randomGraph)],
                          mapNodes[boost::target(*ei, randomGraph)],
                          pointerType);
    }
}


void GenerateGraphWidget::generateErdosRenyiRandomGraph(int nodes, double edgeProbability, int seed, bool selfEdges, int pointerType, int dataType, const QString &identifier)
{
    QPointF center = DocumentManager::self().activeDocument()->activeDataStructure()->relativeCenter();

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    // generate graph
    typedef boost::erdos_renyi_iterator<boost::mt19937, Graph> ergen;
    Graph randomGraph(ergen(gen, nodes, edgeProbability, selfEdges), ergen(), nodes);

    // generate distribution topology and apply
    boost::rectangle_topology< boost::mt19937 > topology(gen, center.x() - 20 * nodes, center.y() - 20 * nodes, center.x() + 20 * nodes, center.y() + 20 * nodes);
    PositionVec position_vec(boost::num_vertices(randomGraph));
    PositionMap positionMap(position_vec.begin(), get(boost::vertex_index, randomGraph));
    boost::random_graph_layout(randomGraph, positionMap, topology);

    // put generated random graph at whiteboard
    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    // minimize cuts by Fruchtman-Reingold layout algorithm
    boost::fruchterman_reingold_force_directed_layout< boost::rectangle_topology< boost::mt19937 >, Graph, PositionMap >
    (randomGraph,
     positionMap,
     topology,
     boost::cooling(boost::linear_cooling<double>(100))
    );

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index = 0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = graph->addData(
                            QString("%1").arg(index++),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1]),
                            dataType
                        );
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        graph->addPointer(mapNodes[boost::source(*ei, randomGraph)],
                          mapNodes[boost::target(*ei, randomGraph)],
                          pointerType);
    }
}


void GenerateGraphWidget::generateRandomTreeGraph(int nodes, int seed, int pointerType, int dataType, const QString &identifier)
{

    Document *activeDocument = DocumentManager::self().activeDocument();
    QPointF center = activeDocument->activeDataStructure()->relativeCenter();

    DataStructurePtr graph = activeDocument->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier);
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed));

    DataList addedNodes;
    addedNodes << graph->addData(QString::number(1), dataType);
    PointerTypePtr ptrType = activeDocument->pointerType(0);
    for (int i = 1; i < nodes; i++) {
        DataPtr thisNode = graph->addData(QString::number(i + 1), center, dataType);
        center += QPointF(30,30);
        boost::random::uniform_int_distribution<> randomEarlierNodeGen(0, i-1);
        int randomEarlierNode = randomEarlierNodeGen(gen);
        graph->addPointer(thisNode, addedNodes.at(randomEarlierNode), pointerType);
        if (ptrType->direction() == PointerType::Unidirectional) {
            graph->addPointer(addedNodes.at(randomEarlierNode), thisNode, pointerType);
        }
        addedNodes.append(thisNode);
    }

    Topology topology = Topology();
    topology.directedGraphDefaultTopology(graph);
}

#include "GenerateGraphWidget.moc"
