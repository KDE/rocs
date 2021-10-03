/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edgewrapper.h"
#include "documentwrapper.h"
#include "edgetype.h"
#include "edge.h"
#include "edgetypestyle.h"
#include <KLocalizedString>
#include <QDebug>
#include <QEvent>

using namespace GraphTheory;

EdgeWrapper::EdgeWrapper(EdgePtr edge, DocumentWrapper *documentWrapper)
    : m_edge(edge)
    , m_documentWrapper(documentWrapper)
{
    connect(m_edge.data(), &Edge::styleChanged, this, [=] () {
        Q_EMIT colorChanged(m_edge->type()->style()->color());
    } );
    connect(m_edge.data(), &Edge::dynamicPropertiesChanged, this, &EdgeWrapper::updateDynamicProperties);
    connect(m_edge.data(), &Edge::typeChanged, this, &EdgeWrapper::typeChanged);
    updateDynamicProperties();
}

EdgeWrapper::~EdgeWrapper()
{

}

EdgePtr EdgeWrapper::edge() const
{
    return m_edge;
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
    const auto edgeTypes = m_edge->from()->document()->edgeTypes();
    for (const EdgeTypePtr &type : edgeTypes) {
        if (type->id() == typeId) {
            newType = type;
            break;
        }
    }
    if (newType == m_edge->type()) {
        QString command = QString("edge.type = %1)").arg(typeId);
        Q_EMIT message(i18nc("@info:shell", "%1: edge type ID %2 not registered", command, typeId), Kernel::ErrorMessage);
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

bool EdgeWrapper::event(QEvent *e)
{
    if (e->type() == QEvent::DynamicPropertyChange) {
        QDynamicPropertyChangeEvent *propertyEvent = static_cast<QDynamicPropertyChangeEvent *>(e);
        QString name = QString::fromUtf8(propertyEvent->propertyName());
        QVariant value = property(propertyEvent->propertyName());
        // only propagate property to edge object if it is registered
        if (m_edge->dynamicProperties().contains(name)) {
            m_edge->setDynamicProperty(name, value);
        }
        return true;
    }
    return QObject::event(e);
}

void EdgeWrapper::updateDynamicProperties()
{
    const auto dynamicProperties = m_edge->dynamicProperties();
    for (const QString &property : dynamicProperties) {
        // property value must not be set to QVariant::Invalid, else the properties are not accessible
        // from the script engine
        if (m_edge->dynamicProperty(property).isValid()) {
            setProperty(property.toUtf8(), m_edge->dynamicProperty(property));
        } else {
            setProperty(property.toUtf8(), QVariant::Int);
        }
    }
}
