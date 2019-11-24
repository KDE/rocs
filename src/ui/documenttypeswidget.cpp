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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
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

    connect(nodeDelegate, &NodeTypesDelegate::colorChanged,
        this, &DocumentTypesWidget::onNodeTypeColorChanged);
    connect(nodeDelegate, &NodeTypesDelegate::nameChanged,
        this, &DocumentTypesWidget::onNodeTypeNameChanged);
    connect(m_createNodeTypeButton, &QPushButton::clicked,
        this, &DocumentTypesWidget::onCreateNodeType);

    // edge types
    layout->addWidget(new QLabel(i18nc("@title", "Edge Types")));

    m_createEdgeTypeButton->setText(i18n("Create Type"));
    layout->addWidget(m_createEdgeTypeButton);

    QListView *edgeTypeView = new QListView(this);
    EdgeTypesDelegate *edgeDelegate = new EdgeTypesDelegate(edgeTypeView);
    edgeTypeView->setItemDelegate(edgeDelegate);
    edgeTypeView->setModel(&m_edgeTypeModel);
    layout->addWidget(edgeTypeView);

    connect(edgeDelegate, &EdgeTypesDelegate::colorChanged,
        this, &DocumentTypesWidget::onEdgeTypeColorChanged);
    connect(edgeDelegate, &EdgeTypesDelegate::nameChanged,
        this, &DocumentTypesWidget::onEdgeTypeNameChanged);
    connect(edgeDelegate, &EdgeTypesDelegate::directionChanged,
        this, &DocumentTypesWidget::onEdgeTypeDirectionChanged);
    connect(m_createEdgeTypeButton, &QPushButton::clicked,
        this, &DocumentTypesWidget::onCreateEdgeType);

    setLayout(layout);
    show();
}

void DocumentTypesWidget::setDocument(GraphDocumentPtr document)
{
    m_createNodeTypeButton->setEnabled(document ? true : false);
    m_createEdgeTypeButton->setEnabled(document ? true : false);

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

void DocumentTypesWidget::onEdgeTypeDirectionChanged(const QModelIndex& index, const EdgeType::Direction& direction)
{
    m_edgeTypeModel.setData(index, direction, EdgeTypeModel::DirectionRole);
}

void DocumentTypesWidget::onCreateEdgeType()
{
    EdgeTypePtr type = EdgeType::create(m_document);
    type->setName(i18n("unnamed"));
}
