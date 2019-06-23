/*
 *  Copyright 2011-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "generategraphwidget.h"

#include "typenames.h"
#include "graphdocument.h"
#include "edge.h"
#include "modifiers/topology.h"
#include "logging_p.h"

#include <KLocalizedString>
#include <KComboBox>

#include <QDateTime>
#include <QList>
#include <QMap>
#include <QPair>
#include <QButtonGroup>

#include <cmath>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/graph/erdos_renyi_generator.hpp>
#include <boost/math/constants/constants.hpp>

using namespace GraphTheory;

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

// handle boost exceptions
namespace boost {
    void throw_exception(std::exception const &e) {
        qCCritical(GRAPHTHEORY_GENERAL) << "Exception:" << e.what();
    }
}


GenerateGraphWidget::GenerateGraphWidget(GraphDocumentPtr document, QWidget *parent)
    : QDialog(parent)
    , m_document(document)
    , m_seed(1)
{
    // setup default identifiers for the created graphs
    m_defaultIdentifiers.insert(MeshGraph, "MeshGraph");
    m_defaultIdentifiers.insert(StarGraph, "StarGraph");
    m_defaultIdentifiers.insert(CircleGraph, "CircleGraph");
    m_defaultIdentifiers.insert(ErdosRenyiRandomGraph, "RandomGraph");
    m_defaultIdentifiers.insert(RandomTree, "RandomTree");
    m_defaultIdentifiers.insert(PathGraph, "PathGraph");
    m_defaultIdentifiers.insert(CompleteGraph, "CompleteGraph");
    m_defaultIdentifiers.insert(CompleteBipartiteGraph, "CompleteBipartite");

    // set default graph
    m_graphGenerator = MeshGraph;

    setWindowTitle(i18nc("@title:window", "Generate Graph"));
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QWidget *widget = new QWidget(this);
    ui = new Ui::GenerateGraphWidget;
    ui->setupUi(widget);
    mainLayout->addWidget(widget);

    ui->buttonShowAdvanced->setIcon(QIcon::fromTheme("rocsadvancedsetup"));

    connect(ui->buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(this, &QDialog::accepted, this, &GenerateGraphWidget::generateGraph);
    connect(ui->comboGraphGenerator, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GenerateGraphWidget::setGraphGenerator);
    connect(ui->nodeTypeSelector, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GenerateGraphWidget::setNodeType);
    connect(ui->edgeTypeSelector, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &GenerateGraphWidget::setEdgeType);

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

    for (int i = 0; i < document->edgeTypes().length(); ++i) {
        EdgeTypePtr type = document->edgeTypes().at(i);
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            type->name(),
            i);
        ui->edgeTypeSelector->addItem(item, QVariant(i));
    }
    ui->edgeTypeSelector->setCurrentIndex(0);

    for (int i = 0; i < document->nodeTypes().length(); ++i) {
        NodeTypePtr type = document->nodeTypes().at(i);
        QString item = i18nc(
            "@item:inlistbox",
            "%1 (ID %2)",
            type->name(),
            i);
        ui->nodeTypeSelector->addItem(item, QVariant(i));
    }
    ui->nodeTypeSelector->setCurrentIndex(0);
}

void GenerateGraphWidget::setGraphGenerator(int index)
{
    m_graphGenerator = GraphGenerator(index);
    if (m_defaultIdentifiers.contains(m_graphGenerator)) {
        ui->identifier->setText(m_defaultIdentifiers[m_graphGenerator]);
    } else {
        ui->identifier->setText("Graph");
    }
}

void GenerateGraphWidget::setGraphIdentifier(const QString &identifier)
{
    m_identifier = identifier;
}

void GenerateGraphWidget::setNodeType(int type)
{
    if (type >= m_document->nodeTypes().length()) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Node type " << type << " does not exist: aborting";
        return;
    }
    m_nodeType = m_document->nodeTypes().at(type);
}

void GenerateGraphWidget::setEdgeType(int type)
{
    if (type >= m_document->edgeTypes().length()) {
        qCWarning(GRAPHTHEORY_GENERAL) << "Edge type " << type << " does not exist: aborting";
        return;
    }
    m_edgeType = m_document->edgeTypes().at(type);
}

void GenerateGraphWidget::setSeed(int seed)
{
    m_seed = seed;
}

void GenerateGraphWidget::generateGraph()
{
    setGraphIdentifier(ui->identifier->text());

    switch (m_graphGenerator) {
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
        break;
    case PathGraph:
        generatePathGraph(
            ui->pathNodes->value()
        );
        break;
    case CompleteGraph:
        generateCompleteGraph(
            ui->completeNodes->value()
        );
    case CompleteBipartiteGraph:
        generateCompleteBipartiteGraph(
            ui->completeBipartiteNodesLeft->value(),
            ui->completeBipartiteNodesRight->value()
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

QPointF GenerateGraphWidget::documentCenter() const
{
    QPointF center = QPointF(0, 0);
    qreal xSum = 0;
    qreal ySum = 0;
    int number = m_document->nodes().length();
    foreach (NodePtr node, m_document->nodes()) {
        xSum += node->x();
        ySum += node->y();
    }
    if (number > 0) {
        center.setX(xSum / number);
        center.setY(ySum / number);
    }
    return center;
}

void GenerateGraphWidget::generateMesh(int rows, int columns)
{
    QPointF center = documentCenter();

    if (rows < 1) {
        rows = 1;
    }
    if (columns < 1) {
        columns = 1;
    }

    // create mesh of NxN points
    QMap<QPair<int, int>, NodePtr > meshNodes;

    // create mesh nodes, store them in map
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            NodePtr node = Node::create(m_document);
            node->setX(i * 50 - (int)25 * columns + center.x());
            node->setY(j * 50 - (int)25 * rows + center.y());
            node->setType(m_nodeType);
            meshNodes[qMakePair(i, j)] = node;
        }
    }

    // connect mesh nodes
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            if (j < columns - 1) { // horizontal edges
                EdgePtr edge = Edge::create(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i, j + 1)]);
                edge->setType(m_edgeType);
            }
            if (i < rows - 1) { // vertical edges
                EdgePtr edge = Edge::create(meshNodes[qMakePair(i, j)], meshNodes[qMakePair(i + 1, j)]);
                edge->setType(m_edgeType);
            }
        }
    }
}

void GenerateGraphWidget::generateStar(int satelliteNodes)
{
    QPointF center = documentCenter();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * satelliteNodes / (2 * boost::math::constants::pi<double>());

    NodeList nodes;
    for (int i = 1; i <= satelliteNodes; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(sin(i * 2 * boost::math::constants::pi<double>() / satelliteNodes)*radius + center.x());
        node->setY(cos(i * 2 * boost::math::constants::pi<double>() / satelliteNodes)*radius + center.y());
        node->setType(m_nodeType);
        nodes.append(node);
    }

    // center
    NodePtr node = Node::create(m_document);
    node->setX(center.x());
    node->setY(center.y());
    node->setType(m_nodeType);
    nodes.prepend(node);

    // connect circle nodes
    for (int i = 1; i <= satelliteNodes; ++i) {
        EdgePtr edge = Edge::create(nodes.at(0), nodes.at(i));
        edge->setType(m_edgeType);
    }
}

void GenerateGraphWidget::generateCircle(int number)
{
    QPointF center = documentCenter();

    // compute radius such that nodes have space ~50 between each other
    // circle that border-length of 2*PI*radius
    int radius = 50 * number / (2 * boost::math::constants::pi<double>());

    QList< QPair<QString, QPointF> > circleNodes;

    // create mesh nodes, store them in map
    NodeList nodes;
    for (int i = 1; i <= number; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(sin(i * 2 * boost::math::constants::pi<double>() / number)*radius + center.x());
        node->setY(cos(i * 2 * boost::math::constants::pi<double>() / number)*radius + center.y());
        node->setType(m_nodeType);
        nodes.append(node);
    }

    // connect circle nodes
    for (int i = 0; i < number - 1; i++) {
        EdgePtr edge = Edge::create(nodes.at(i), nodes.at(i + 1));
        edge->setType(m_edgeType);
    }
    EdgePtr edge = Edge::create(nodes.at(number - 1), nodes.at(0));
    edge->setType(m_edgeType);
}

void GenerateGraphWidget::generateRandomGraph(int nodes, int edges, bool selfEdges)
{
    QPointF center = documentCenter();

    Graph randomGraph;
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(m_seed));

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

    // put nodes at whiteboard as generated
    QMap<int, NodePtr > mapNodes;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = Node::create(m_document);
        mapNodes[*vi]->setX(positionMap[*vi][0]);
        mapNodes[*vi]->setY(positionMap[*vi][1]);
        mapNodes[*vi]->setType(m_nodeType);
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        EdgePtr edge = Edge::create(mapNodes[boost::source(*ei, randomGraph)], mapNodes[boost::target(*ei, randomGraph)]);
        edge->setType(m_edgeType);
    }
}

void GenerateGraphWidget::generateErdosRenyiRandomGraph(int nodes, double edgeProbability, bool selfEdges)
{
    QPointF center = documentCenter();

    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(m_seed));

    // generate graph
    typedef boost::erdos_renyi_iterator<boost::mt19937, Graph> ergen;
    Graph randomGraph(ergen(gen, nodes, edgeProbability, selfEdges), ergen(), nodes);

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

    // put nodes at whiteboard as generated
    QMap<int, NodePtr > mapNodes;
    boost::graph_traits<Graph>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(randomGraph); vi != vi_end; ++vi) {
        mapNodes[*vi] = Node::create(m_document);
        mapNodes[*vi]->setX(positionMap[*vi][0]);
        mapNodes[*vi]->setY(positionMap[*vi][1]);
        mapNodes[*vi]->setType(m_nodeType);
    }

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(randomGraph); ei != ei_end; ++ei) {
        EdgePtr edge = Edge::create(mapNodes[boost::source(*ei, randomGraph)], mapNodes[boost::target(*ei, randomGraph)]);
        edge->setType(m_edgeType);
    }
}

void GenerateGraphWidget::generateRandomTreeGraph(int number)
{
    boost::mt19937 gen;
    gen.seed(static_cast<unsigned int>(m_seed));

    NodeList nodes;

    NodePtr node = Node::create(m_document);
    node->setType(m_nodeType);
    nodes.append(node);

    for (int i = 1; i < number; ++i) {
        NodePtr thisNode = Node::create(m_document);
        node->setType(m_nodeType);
        boost::random::uniform_int_distribution<> randomEarlierNodeGen(0, i-1);
        int randomEarlierNode = randomEarlierNodeGen(gen);
        EdgePtr edge = Edge::create(thisNode, nodes.at(randomEarlierNode));
        edge->setType(m_edgeType);
        if (m_edgeType->direction() == EdgeType::Unidirectional) {
            edge = Edge::create(nodes.at(randomEarlierNode), thisNode);
            edge->setType(m_edgeType);
        }
        nodes.append(thisNode);
    }

    Topology topology = Topology();
    topology.directedGraphDefaultTopology(m_document);
}

void GenerateGraphWidget::generatePathGraph(int pathSize)
{
    QPointF center = documentCenter();

    QList< QPair<QString, QPointF> > pathNodes;


    NodeList nodes_list;
    for (int i = 1; i <= pathSize; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(i * 50 + center.x());
        node->setY(center.y());
        node->setType(m_nodeType);
        nodes_list.append(node);
    }

    for (int i = 0; i < pathSize - 1; i++) {
        EdgePtr edge = Edge::create(nodes_list.at(i), nodes_list.at(i + 1));
        edge->setType(m_edgeType);
    }
}

void GenerateGraphWidget::generateCompleteGraph(int nodes)
{
    QPointF center = documentCenter();

    // compute radius such that nodes have space ~100 between each other
    // circle that border-length of 2*PI*radius
    int radius = 100 * nodes / (2 * boost::math::constants::pi<double>());

    QList< QPair<QString, QPointF> > circleNodes;

    NodeList node_list;
    for (int i = 1; i <= nodes; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(sin(i * 2 * boost::math::constants::pi<double>() / nodes)*radius + center.x());
        node->setY(cos(i * 2 * boost::math::constants::pi<double>() / nodes)*radius + center.y());
        node->setType(m_nodeType);
        node_list.append(node);
    }

    for (int i = 0; i < nodes - 1; i++) {
        for (int j = i + 1; j < nodes; j++){
            EdgePtr edge_lr = Edge::create(node_list.at(i), node_list.at(j));
            EdgePtr edge_rl = Edge::create(node_list.at(j), node_list.at(i));

            edge_lr->setType(m_edgeType);
            edge_rl->setType(m_edgeType);
        }
    }
}

void GenerateGraphWidget::generateCompleteBipartiteGraph(int nodes_left, int nodes_right)
{
    QPointF center = documentCenter();

    int separator = 100;
    NodeList node_list;

    for (int i = 0; i < nodes_left; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(center.x());
        node->setY(center.y() + i * 50);
        node->setType(m_nodeType);
        node_list.append(node);
    }

    for (int i = 0; i < nodes_right; i++) {
        NodePtr node = Node::create(m_document);
        node->setX(center.x() + separator);
        node->setY(center.y() + i * 50);
        node->setType(m_nodeType);
        node_list.append(node);
    }

    for (int i = 0; i < nodes_left; i++) {
        for (int j = 0; j < nodes_right; j++){
            EdgePtr edge_lr = Edge::create(node_list.at(i), node_list.at(j + nodes_left));
            EdgePtr edge_rl = Edge::create(node_list.at(j + nodes_left), node_list.at(i));

            edge_lr->setType(m_edgeType);
            edge_rl->setType(m_edgeType);
        }
    }
}
