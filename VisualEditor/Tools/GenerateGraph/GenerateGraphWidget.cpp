/*
    This file is part of Rocs.
    Copyright (C) 2011-2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <KLocalizedString>
#include <KComboBox>

#include <QList>
#include <QMap>
#include <QPair>
#include <QDebug>
#include <QButtonGroup>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/math/constants/constants.hpp>

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

// workaround for linking boost
void boost::throw_exception(std::exception const & e)
{
}


GenerateGraphWidget::GenerateGraphWidget(Document *document)
    : QDialog()
{
    // setup default identifiers for the created graphs
    defaultIdentifiers.insert(MeshGraph, "MeshGraph");
    defaultIdentifiers.insert(StarGraph, "StarGraph");
    defaultIdentifiers.insert(CircleGraph, "CircleGraph");
    defaultIdentifiers.insert(ErdosRenyiRandomGraph, "RandomGraph");
    defaultIdentifiers.insert(RandomTree, "RandomTree");
    defaultIdentifiers.insert(MeshGraph, "MeshGraph");
    graphGenerator_ = MeshGraph;

    setWindowTitle(i18nc("@title:window", "Generate Graph"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QWidget *widget = new QWidget(this);
    ui = new Ui::GenerateGraphWidget;
    ui->setupUi(widget);
    mainLayout->addWidget(widget);

    ui->buttonShowAdvanced->setIcon(QIcon::fromTheme("rocsadvancedsetup"));

    connect(this, SIGNAL(accepted()), this, SLOT(generateGraph()));
    connect(ui->comboGraphGenerator, SIGNAL(currentIndexChanged(int)), this, SLOT(setGraphGenerator(int)));
    connect(ui->dataTypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(setDataType(int)));
    connect(ui->pointerTypeSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(setPointerType(int)));

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


void GenerateGraphWidget::setGraphGenerator(int index)
{
    graphGenerator_ = GraphGenerator(index);
    if (defaultIdentifiers.contains(graphGenerator_)) {
        ui->identifier->setText(defaultIdentifiers[graphGenerator_]);
    } else {
        ui->identifier->setText("Graph");
    }
}

void GenerateGraphWidget::setGraphIdentifier(const QString &identifier)
{
    identifier_ = identifier;
}

void GenerateGraphWidget::setDataType(int type)
{
    if (!DocumentManager::self().activeDocument()->dataTypeList().contains(type)) {
        qWarning() << "Data type " << type << " does not exist: aborting";
        return;
    }
    dataType_ = type;
}

void GenerateGraphWidget::setPointerType(int type)
{
    if (!DocumentManager::self().activeDocument()->pointerTypeList().contains(type)) {
        qWarning() << "Pointer type " << type << " does not exist: aborting";
        return;
    }
    pointerType_ = type;
}

void GenerateGraphWidget::setSeed(int seed)
{
    seed_ = seed;
}

void GenerateGraphWidget::generateGraph()
{
    setGraphIdentifier(ui->identifier->text());

    switch (graphGenerator_) {
    case MeshGraph:
        generateMesh(ui->meshRows->value(), ui->meshColumns->value());
        break;
    case CircleGraph:
        generateCircle(ui->circleNodes->value());
        break;
    case StarGraph:
        generateStar(ui->starSatelliteNodes->value());
        break;
    case RandomEdgeGraph:
        setSeed(ui->randomGeneratorSeed->value());
        generateRandomGraph(
            ui->randomNodes->value(),
            ui->randomEdges->value(),
            ui->randomAllowSelfedges->isTristate()
        );
        break;
    case ErdosRenyiRandomGraph:
        setSeed(ui->randomGeneratorSeed->value());
        generateErdosRenyiRandomGraph(
            ui->GNPNodes->value(),
            ui->GNPEdgeProbability->value(),
            ui->GNPAllowSelfedges->isTristate()
        );
        break;
    case RandomTree:
        setSeed(ui->randomTreeGeneratorSeed->value());
        generateRandomTreeGraph(
            ui->randomTreeNodes->value()
        );
    default:
        break;
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
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    if (rows < 1) {
        rows = 1;
    }
    if (columns < 1) {
        columns = 1;
    }

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
    }

    // create mesh of NxN points
    QMap<QPair<int, int>, DataPtr > meshNodes;

    // create mesh nodes, store them in map
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            meshNodes[qMakePair(i, j)] = graph->createData(QString("%1-%2").arg(i).arg(j),
                                                        QPointF(i * 50, j * 50) - QPoint((int)25 * columns, (int)25 * rows) + center,
                                                        dataType_
                                                       );
        }
    }

    // connect mesh nodes
    for (int i = 0; i < columns; i++) {
        for (int j = 0; j < rows; j++) {
            graph->createPointer(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i, j + 1)], pointerType_); // left
            graph->createPointer(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i + 1, j)], pointerType_); // bottom.
        }
    }
}

void GenerateGraphWidget::generateStar(int satelliteNodes)
{
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * satelliteNodes / (2 * boost::math::constants::pi<double>());

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
    }

    QList< QPair<QString, QPointF> > starNodes;
    for (int i = 1; i <= satelliteNodes; i++) {
        QPointF position = QPointF(sin(i * 2 * boost::math::constants::pi<double>() / satelliteNodes)*radius,
                                    cos(i * 2 * boost::math::constants::pi<double>() / satelliteNodes)*radius)
                            + center;
        starNodes << qMakePair(
                      QString("%1").arg(i),
                      position
                  );
    }
    QList< DataPtr > nodeList = graph->addDataList(starNodes, dataType_);

    // center
    nodeList.prepend(graph->createData(QString("center"), center, dataType_));

    // connect circle nodes
    for (int i = 1; i <= satelliteNodes; i++) {
        graph->createPointer(nodeList.at(0), nodeList.at(i), pointerType_);
    }
}

void GenerateGraphWidget::generateCircle(int nodes)
{
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * nodes / (2 * boost::math::constants::pi<double>());

    // use active data structure iff empty
    DataStructurePtr graph = DocumentManager::self().activeDocument()->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
    }

    QList< QPair<QString, QPointF> > circleNodes;

    // create mesh nodes, store them in map
    for (int i = 1; i <= nodes; i++) {
        QPointF position = QPointF( sin(i * 2 * boost::math::constants::pi<double>() / nodes)*radius,
                                    cos(i * 2 * boost::math::constants::pi<double>() / nodes)*radius)
                           + center;
        circleNodes << qMakePair(
                        QString("%1").arg(i),
                        position
                    );
    }
    QList< DataPtr > nodeList = graph->addDataList(circleNodes, dataType_);

    // connect circle nodes
    for (int i = 0; i < nodes - 1; i++) {
        graph->createPointer(nodeList.at(i), nodeList.at(i + 1), pointerType_);
    }
    graph->createPointer(nodeList.at(nodes - 1), nodeList.at(0), pointerType_);
}

void GenerateGraphWidget::generateRandomGraph(int nodes, int edges, bool selfEdges)
{
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    Graph randomGraph;
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed_));

    // generate graph
    boost::generate_random_graph<Graph, boost::mt19937>(
        randomGraph,
        nodes,
        edges,
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
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
    }

    // put nodes at whiteboard as generated
    QMap<int, DataPtr > mapNodes;
    int index = 0;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = graph->createData(
                            QString("%1").arg(index++),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1]),
                            dataType_
                        );
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        graph->createPointer(mapNodes[boost::source(*ei, randomGraph)],
                          mapNodes[boost::target(*ei, randomGraph)],
                          pointerType_);
    }
}


void GenerateGraphWidget::generateErdosRenyiRandomGraph(int nodes, double edgeProbability, bool selfEdges)
{
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed_));

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
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
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
        mapNodes[*vi] = graph->createData(
                            QString("%1").arg(index++),
                            QPointF(positionMap[*vi][0], positionMap[*vi][1]),
                            dataType_
                        );
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        graph->createPointer(mapNodes[boost::source(*ei, randomGraph)],
                          mapNodes[boost::target(*ei, randomGraph)],
                          pointerType_);
    }
}


void GenerateGraphWidget::generateRandomTreeGraph(int nodes)
{
    Document *activeDocument = DocumentManager::self().activeDocument();
    QPointF center = DocumentManager::self().activeDocument()->sceneRect().center();

    DataStructurePtr graph = activeDocument->activeDataStructure();
    if (graph->dataListAll().size() > 0) {
        graph = DocumentManager::self().activeDocument()->addDataStructure(identifier_);
    }

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(seed_));

    DataList addedNodes;
    addedNodes << graph->createData(QString::number(1), dataType_);
    PointerTypePtr ptrType = activeDocument->pointerType(0);
    for (int i = 1; i < nodes; i++) {
        DataPtr thisNode = graph->createData(QString::number(i + 1), center, dataType_);
        center += QPointF(30,30);
        boost::random::uniform_int_distribution<> randomEarlierNodeGen(0, i-1);
        int randomEarlierNode = randomEarlierNodeGen(gen);
        graph->createPointer(thisNode, addedNodes.at(randomEarlierNode), pointerType_);
        if (ptrType->direction() == PointerType::Unidirectional) {
            graph->createPointer(addedNodes.at(randomEarlierNode), thisNode, pointerType_);
        }
        addedNodes.append(thisNode);
    }

    Topology topology = Topology();
    topology.directedGraphDefaultTopology(graph);
}
