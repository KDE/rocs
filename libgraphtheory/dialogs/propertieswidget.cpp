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
    , m_type(NodeTypePtr())
    , m_model(new NodeTypePropertyModel(this))
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
    m_view->setModel(m_model);

    connect(delegate, &PropertyDelegate::deleteProperty,
        this, &PropertiesWidget::deleteProperty);
    connect(delegate, &PropertyDelegate::nameChanged,
        this, &PropertiesWidget::renameProperty);
}

void PropertiesWidget::setNodeType(NodeTypePtr type)
{
    m_model->setNodeType(type.data());
    m_type = type;
}

void PropertiesWidget::addProperty()
{
    if (!m_type) {
        return;
    }
    QStringList propertyNames = m_type->dynamicProperties();
    QString name = "property";
    if (propertyNames.contains(name)) {
        int suffix = 1;
        while (propertyNames.contains(name + QString::number(suffix))) {
            ++suffix;
        }
        name += QString::number(suffix);
    }
    m_type->addDynamicProperty(name);
}

void PropertiesWidget::deleteProperty(const QModelIndex &index)
{
    m_type->removeDynamicProperty(m_model->data(index, NodeTypePropertyModel::NameRole).toString());
}

void PropertiesWidget::renameProperty(const QModelIndex& index, const QString& name)
{
    m_type->renameDynamicProperty(m_model->data(index, NodeTypePropertyModel::NameRole).toString(), name);
}
