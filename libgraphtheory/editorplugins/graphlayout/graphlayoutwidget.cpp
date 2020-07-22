/*
 *  Copyright 2020       Dilson Almeida Guimarães <dilsonguim@gmail.com>
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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "graphlayoutwidget.h"

#include "typenames.h"
#include "graphdocument.h"
#include "edge.h"
#include "modifiers/topology.h"
#include "logging_p.h"

#include <KLocalizedString>
#include <KComboBox>

#include <QtMath>
#include <QSlider>
#include <QList>
#include <QButtonGroup>
#include <QMessageBox>

using namespace GraphTheory;


GraphLayoutWidget::GraphLayoutWidget(GraphDocumentPtr document, QWidget *parent)
    : QDialog(parent)
    , m_document(document)
    , m_seed(1)
    , m_areaFactor(50)
    , m_repellingForce(50)
    , m_attractionForce(50)
    , m_currentTabIndex(0)
    , m_root(-1)
    , m_nodeSeparation(50)
    , m_treeType(TreeType::Free)
{
    setWindowTitle(i18nc("@title:window", "Graph Layout"));
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QWidget *widget = new QWidget(this);
    ui = new Ui::GraphLayoutWidget;
    ui->setupUi(widget);
    mainLayout->addWidget(widget);

    connect(ui->tabs, &QTabWidget::currentChanged, this, &GraphLayoutWidget::setCurrentTabIndex);
    connect(ui->mainButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->mainButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(this, &QDialog::accepted, this, &GraphLayoutWidget::layoutGraph);
    connect(ui->areaFactorSlider, &QSlider::valueChanged, this, &GraphLayoutWidget::setAreaFactor);
    connect(ui->repellingForceSlider, &QSlider::valueChanged, this,
            &GraphLayoutWidget::setRepellingForce);
    connect(ui->attractionForceSlider, &QSlider::valueChanged, this,
            &GraphLayoutWidget::setAttractionForce);

    connect(ui->nodeSeparationSlider, &QSlider::valueChanged, this,
            &GraphLayoutWidget::setNodeSeparation);
    connect(ui->rootComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &GraphLayoutWidget::setRoot);
    
    connect(ui->freeTreeRadioButton, &QRadioButton::toggled, this,
            &GraphLayoutWidget::freeTreeTypeToggle);
    connect(ui->rootedTreeRadioButton, &QRadioButton::toggled, this,
            &GraphLayoutWidget::rootedTreeTypeToggle);

    //Adds items to center/root combo box in the radial layout tab
    ui->rootComboBox->addItem("Automatic selection", QVariant(-1));
    for (const auto nodePtr : document->nodes()) {
        const int nodeId = nodePtr->id();
        ui->rootComboBox->addItem(QString::number(nodeId), QVariant(nodeId));
    }

    // default values
    ui->tabs->setCurrentIndex(m_currentTabIndex);
    ui->areaFactorSlider->setValue(50);
    ui->repellingForceSlider->setValue(50);
    ui->attractionForceSlider->setValue(50);
    ui->nodeSeparationSlider->setValue(50);
}


void GraphLayoutWidget::setAreaFactor(int areaFactor) {
    m_areaFactor = areaFactor;
}

void GraphLayoutWidget::setRepellingForce(int repellingForce) {
    m_repellingForce = repellingForce;
}

void GraphLayoutWidget::setAttractionForce(int attractionForce) {
    m_attractionForce = attractionForce;
}


void GraphLayoutWidget::setSeed(int seed)
{
    m_seed = seed;
}

void GraphLayoutWidget::setCurrentTabIndex(const int index)
{
    m_currentTabIndex = index;
}

void GraphLayoutWidget::layoutGraph()
{

    const QString currentTabName = getCurrentTabName();
    if (currentTabName == "forceBasedLayoutTab") {
        handleForceBasedLayout();
    } else if (currentTabName == "radialTreeLayoutTab") {
        handleRadialTreeLayout();
    }

    close();
    deleteLater();
}

void GraphLayoutWidget::handleForceBasedLayout()
{
    //Slider values map to parameters with a non-linear scale
    const qreal areaFactor = qPow(10, qreal(m_areaFactor - 50) / 50);
    const qreal repellingForce = qPow(10, qreal(m_repellingForce - 50) / 50);
    const qreal attractionForce = qPow(10, qreal(m_attractionForce - 50) / 50);
   
    //Creates a small margin to make the layout look nicer.
    const qreal margin = 5;

    //Radius of each node. This should be equal to radius used when rendering the graph.
    const qreal nodeRadius = 10;


    const bool randomizeInitialPositions = true;
    const quint32 seed = m_seed;

    Topology::applyForceBasedLayout(m_document, nodeRadius, margin, areaFactor, repellingForce,
                                    attractionForce, randomizeInitialPositions, seed);
}

void GraphLayoutWidget::handleRadialTreeLayout()
{
    //TODO: Check if the graph is a tree.

    //Finds the root node. In case of automatic selection, a null pointer is used.
    NodePtr root = nullptr;
    for (const NodePtr node : m_document->nodes()) {
        if (node->id() == m_root) {
            root = node;
            break;
        }
    }

    const qreal nodeRadius = 10.;
    const qreal margin = 5.;
    const qreal nodeSeparation = m_nodeSeparation;

    if (m_treeType == TreeType::Free) {
        const qreal wedgeAngle = 2. * M_PI;
        const qreal rotationAngle = 0.;
        
        Topology::applyRadialLayoutToTree(m_document, nodeRadius, margin, nodeSeparation, root,
                                          wedgeAngle, rotationAngle);
    } else {
        const qreal wedgeAngle = M_PI / 2.;
        const qreal rotationAngle = (M_PI - wedgeAngle) / 2.;
        
        Topology::applyRadialLayoutToTree(m_document, nodeRadius, margin, nodeSeparation, root,
                                          wedgeAngle, rotationAngle);
    }
}

void GraphLayoutWidget::setNodeSeparation(const int nodeSeparation)
{
    m_nodeSeparation = nodeSeparation;
}

void GraphLayoutWidget::setRoot(const int index)
{
    m_root = ui->rootComboBox->itemData(index).toInt();
}

void GraphLayoutWidget::freeTreeTypeToggle(const bool checked)
{
    if (checked) {
        m_treeType = TreeType::Free;
    }
}

void GraphLayoutWidget::rootedTreeTypeToggle(const bool checked)
{
    if (checked) {
        m_treeType = TreeType::Rooted;
    }
}

QString GraphLayoutWidget::getCurrentTabName() const
{
    const QWidget* currentTab = ui->tabs->widget(m_currentTabIndex);
    return currentTab->objectName();
}

GraphLayoutWidget::~GraphLayoutWidget()
{
    delete ui;
}
