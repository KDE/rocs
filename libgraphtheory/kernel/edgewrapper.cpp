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
    connect(m_edge.data(), SIGNAL(colorChanged(QColor)), this, SIGNAL(colorChanged(QColor)));
    connect(m_edge.data(), SIGNAL(dynamicPropertiesChanged()), this, SLOT(updateDynamicProperties()));

    updateDynamicProperties();
}

EdgeWrapper::~EdgeWrapper()
{

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
