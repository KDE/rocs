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
{
    setWindowTitle(i18nc("@title:window", "Graph Layout"));
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setLayout(mainLayout);

    QWidget *widget = new QWidget(this);
    ui = new Ui::GraphLayoutWidget;
    ui->setupUi(widget);
    mainLayout->addWidget(widget);

    connect(ui->mainButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->mainButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(this, &QDialog::accepted, this, &GraphLayoutWidget::layoutGraph);
    connect(ui->areaFactorSlider, &QSlider::valueChanged, this, &GraphLayoutWidget::setAreaFactor);
    connect(ui->repellingForceSlider, &QSlider::valueChanged, this, &GraphLayoutWidget::setRepellingForce);
    connect(ui->attractionForceSlider, &QSlider::valueChanged, this, &GraphLayoutWidget::setAttractionForce);

    // default values
    ui->areaFactorSlider->setValue(50);
    ui->repellingForceSlider->setValue(50);
    ui->attractionForceSlider->setValue(50);
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

void GraphLayoutWidget::layoutGraph()
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
    
    close();
    deleteLater();
}

GraphLayoutWidget::~GraphLayoutWidget()
{
    delete ui;
}
