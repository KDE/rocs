/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "propertieswidget.h"
#include "edgetype.h"
#include "nodetype.h"
#include "org/kde/rocs/edgetypepropertymodel.h"
#include "org/kde/rocs/nodetypepropertymodel.h"
#include "propertydelegate.h"
#include <KLocalizedString>
#include <QDebug>
#include <QGridLayout>
#include <QLayout>
#include <QListView>
#include <QModelIndex>
#include <QPushButton>

using namespace GraphTheory;

PropertiesWidget::PropertiesWidget(QWidget *parent)
    : QWidget(parent)
    , m_nodeType(NodeTypePtr())
    , m_nodeModel(new NodeTypePropertyModel(this))
    , m_edgeType(EdgeTypePtr())
    , m_edgeModel(new EdgeTypePropertyModel(this))
    , m_view(new QListView(this))
{
    setLayout(new QGridLayout(this));
    layout()->addWidget(m_view);
    QPushButton *addPropertyButton = new QPushButton(i18n("Add Property"));
    connect(addPropertyButton, &QPushButton::clicked, this, &PropertiesWidget::addProperty);
    layout()->addWidget(addPropertyButton);

    PropertyDelegate *delegate = new PropertyDelegate(m_view);
    m_view->setItemDelegate(delegate);

    connect(delegate, &PropertyDelegate::deleteProperty, this, &PropertiesWidget::deleteProperty);
    connect(delegate, &PropertyDelegate::nameChanged, this, &PropertiesWidget::renameProperty);
}

void PropertiesWidget::setType(NodeTypePtr type)
{
    m_nodeModel->setNodeType(type.data());
    m_nodeType = type;
    m_view->setModel(m_nodeModel);
    m_edgeType.reset();
}

void PropertiesWidget::setType(EdgeTypePtr type)
{
    m_edgeModel->setEdgeType(type.data());
    m_edgeType = type;
    m_view->setModel(m_edgeModel);
    m_nodeType.reset();
}

void PropertiesWidget::addProperty()
{
    if (!m_nodeType && !m_edgeType) {
        return;
    }
    QStringList propertyNames;
    if (m_nodeType) {
        propertyNames = m_nodeType->dynamicProperties();
    } else {
        propertyNames = m_edgeType->dynamicProperties();
    }
    QString name = "property";
    if (propertyNames.contains(name)) {
        int suffix = 1;
        while (propertyNames.contains(name + QString::number(suffix))) {
            ++suffix;
        }
        name += QString::number(suffix);
    }
    if (m_nodeType) {
        m_nodeType->addDynamicProperty(name);
    } else {
        m_edgeType->addDynamicProperty(name);
    }
}

void PropertiesWidget::deleteProperty(const QModelIndex &index)
{
    if (m_nodeType) {
        m_nodeType->removeDynamicProperty(m_nodeModel->data(index, NodeTypePropertyModel::NameRole).toString());
    } else {
        m_edgeType->removeDynamicProperty(m_edgeModel->data(index, NodeTypePropertyModel::NameRole).toString());
    }
}

void PropertiesWidget::renameProperty(const QModelIndex &index, const QString &name)
{
    if (m_nodeType) {
        m_nodeType->renameDynamicProperty(m_nodeModel->data(index, NodeTypePropertyModel::NameRole).toString(), name);
    } else {
        m_edgeType->renameDynamicProperty(m_edgeModel->data(index, NodeTypePropertyModel::NameRole).toString(), name);
    }
}

#include "moc_propertieswidget.cpp"
