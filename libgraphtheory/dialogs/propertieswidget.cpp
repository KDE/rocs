/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "propertieswidget.h"
#include "propertydelegate.h"
#include "edgetype.h"
#include "nodetype.h"
#include "models/edgetypepropertymodel.h"
#include "models/nodetypepropertymodel.h"
#include "typenames.h"
#include <KLocalizedString>
#include <QLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QListView>
#include <QModelIndex>
#include <QDebug>

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
    connect(addPropertyButton, &QPushButton::clicked,
        this, &PropertiesWidget::addProperty);
    layout()->addWidget(addPropertyButton);

    PropertyDelegate *delegate = new PropertyDelegate(m_view);
    m_view->setItemDelegate(delegate);

    connect(delegate, &PropertyDelegate::deleteProperty,
        this, &PropertiesWidget::deleteProperty);
    connect(delegate, &PropertyDelegate::nameChanged,
        this, &PropertiesWidget::renameProperty);
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

void PropertiesWidget::renameProperty(const QModelIndex& index, const QString& name)
{
    if (m_nodeType) {
        m_nodeType->renameDynamicProperty(m_nodeModel->data(index, NodeTypePropertyModel::NameRole).toString(), name);
    } else {
        m_edgeType->renameDynamicProperty(m_edgeModel->data(index, NodeTypePropertyModel::NameRole).toString(), name);
    }
}
