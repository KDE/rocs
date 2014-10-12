/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "documenttypeswidget.h"
#include "nodetypesdelegate.h"
#include "edgetypesdelegate.h"
#include "libgraphtheory/models/nodetypemodel.h"
#include "libgraphtheory/models/edgetypemodel.h"
#include "libgraphtheory/nodetype.h"
#include "libgraphtheory/edgetype.h"
#include <KLocalizedString>
#include <KWidgetItemDelegate>
#include <QGridLayout>
#include <QListView>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

using namespace GraphTheory;

DocumentTypesWidget::DocumentTypesWidget(QWidget *parent)
    : QWidget(parent)
    , m_createNodeTypeButton(new QPushButton(this))
    , m_createEdgeTypeButton(new QPushButton(this))
{
    QGridLayout *layout = new QGridLayout(this);

    // node types
    layout->addWidget(new QLabel(i18nc("@title", "Node Types")));

    m_createNodeTypeButton->setText(i18n("Create Type"));
    layout->addWidget(m_createNodeTypeButton);

    QListView *nodeTypeView = new QListView(this);
    NodeTypesDelegate *nodeDelegate = new NodeTypesDelegate(nodeTypeView);
    nodeTypeView->setItemDelegate(nodeDelegate);
    nodeTypeView->setModel(&m_nodeTypeModel);
    layout->addWidget(nodeTypeView);

    connect(nodeDelegate, SIGNAL(colorChanged(QModelIndex,QColor)), this, SLOT(onNodeTypeColorChanged(QModelIndex,QColor)));
    connect(nodeDelegate, SIGNAL(nameChanged(QModelIndex,QString)), this, SLOT(onNodeTypeNameChanged(QModelIndex,QString)));
    connect(m_createNodeTypeButton, SIGNAL(clicked(bool)), this, SLOT(onCreateNodeType()));

    // edge types
    layout->addWidget(new QLabel(i18nc("@title", "Edge Types")));

    m_createEdgeTypeButton->setText(i18n("Create Type"));
    layout->addWidget(m_createEdgeTypeButton);

    QListView *edgeTypeView = new QListView(this);
    EdgeTypesDelegate *edgeDelegate = new EdgeTypesDelegate(edgeTypeView);
    edgeTypeView->setItemDelegate(edgeDelegate);
    edgeTypeView->setModel(&m_edgeTypeModel);
    layout->addWidget(edgeTypeView);

    connect(edgeDelegate, SIGNAL(colorChanged(QModelIndex,QColor)), this, SLOT(onEdgeTypeColorChanged(QModelIndex,QColor)));
    connect(edgeDelegate, SIGNAL(nameChanged(QModelIndex,QString)), this, SLOT(onEdgeTypeNameChanged(QModelIndex,QString)));
    connect(m_createEdgeTypeButton, SIGNAL(clicked(bool)), this, SLOT(onCreateEdgeType()));

    setLayout(layout);
    show();
}

DocumentTypesWidget::~DocumentTypesWidget()
{

}

void DocumentTypesWidget::setDocument(GraphDocumentPtr document)
{
    if (!document) { // deactivate controls if no document exists
        m_createNodeTypeButton->setEnabled(false);
        m_createEdgeTypeButton->setEnabled(false);
    } else {
        m_createNodeTypeButton->setEnabled(true);
        m_createEdgeTypeButton->setEnabled(true);
    }

    m_document = document;
    m_nodeTypeModel.setDocument(document);
    m_edgeTypeModel.setDocument(document);
}

void DocumentTypesWidget::onNodeTypeColorChanged(const QModelIndex &index, const QColor &color)
{
    m_nodeTypeModel.setData(index, QVariant::fromValue<QColor>(color), NodeTypeModel::ColorRole);
}

void DocumentTypesWidget::onNodeTypeNameChanged(const QModelIndex& index, const QString& name)
{
    m_nodeTypeModel.setData(index, QVariant(name), NodeTypeModel::TitleRole);
}

void DocumentTypesWidget::onCreateNodeType()
{
    NodeTypePtr type = NodeType::create(m_document);
    type->setName(i18n("unnamed"));
}

void DocumentTypesWidget::onEdgeTypeColorChanged(const QModelIndex &index, const QColor &color)
{
    m_edgeTypeModel.setData(index, QVariant::fromValue<QColor>(color), EdgeTypeModel::ColorRole);
}

void DocumentTypesWidget::onEdgeTypeNameChanged(const QModelIndex& index, const QString& name)
{
    m_edgeTypeModel.setData(index, QVariant(name), EdgeTypeModel::TitleRole);
}

void DocumentTypesWidget::onCreateEdgeType()
{
    EdgeTypePtr type = EdgeType::create(m_document);
    type->setName(i18n("unnamed"));
}
