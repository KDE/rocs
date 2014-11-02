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

#include "edgewrapper.h"
#include "documentwrapper.h"
#include "graphdocument.h"
#include "edgetype.h"
#include "edge.h"

#include <QColor>
#include <QDebug>

using namespace GraphTheory;

EdgeWrapper::EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper)
    : m_edge(edge)
    , m_documentWrapper(documentWrapper)
{
    connect(m_edge.data(), &Edge::typeColorChanged, this, &EdgeWrapper::colorChanged);
    connect(m_edge.data(), &Edge::dynamicPropertiesChanged, this, &EdgeWrapper::updateDynamicProperties);
    connect(m_edge.data(), &Edge::typeChanged, this, &EdgeWrapper::typeChanged);

    updateDynamicProperties();
}

EdgeWrapper::~EdgeWrapper()
{

}

int EdgeWrapper::type() const
{
    return m_edge->type()->id();
}

void EdgeWrapper::setType(int typeId)
{
    EdgeTypePtr newType = m_edge->type();
    if (newType->id() == typeId) {
        return;
    }
    foreach (EdgeTypePtr type, m_edge->from()->document()->edgeTypes()) {
        if (type->id() == typeId) {
            newType = type;
            break;
        }
    }
    if (newType == m_edge->type()) {
        qWarning() << "No type with ID" << typeId << "found, aborting type change.";
        return;
    }
    m_edge->setType(newType);
    // change signal will be emitted by connection to m_node signal
}

GraphTheory::NodeWrapper * EdgeWrapper::from() const
{
    return m_documentWrapper->nodeWrapper(m_edge->from());
}

GraphTheory::NodeWrapper * EdgeWrapper::to() const
{
    return m_documentWrapper->nodeWrapper(m_edge->to());
}

bool EdgeWrapper::directed() const
{
    if (m_edge->type()->direction() == EdgeType::Unidirectional) {
        return true;
    }
    return false;
}

bool EdgeWrapper::event(QEvent* e)
{
    if (e->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent *>(e);
        QString name = QString::fromUtf8(propertyEvent->propertyName());
        QVariant value = property(propertyEvent->propertyName());
        m_edge->setDynamicProperty(name, value);
    }
    return parent()->event(e);
}

void EdgeWrapper::updateDynamicProperties()
{
    // set dynamic properties
    foreach (QString property, m_edge->dynamicProperties()) {
        setProperty(property.toUtf8(), m_edge->dynamicProperty(property));
    }
}
