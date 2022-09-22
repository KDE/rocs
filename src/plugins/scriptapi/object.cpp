/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "object.h"
#include <QDebug>

Object::Object(QObject *parent)
    : QObject(parent)
    , m_componentType(Object::KernelModule)
{
}

Object::~Object()
{
}

void Object::setId(const QString &id)
{
    m_id = id;
}

QString Object::id() const
{
    return m_id;
}

void Object::setComponentType(const QString &compenentTypeName)
{
    if (compenentTypeName == "Document") {
        m_componentType = Document;
        return;
    }
    if (compenentTypeName == "Node") {
        m_componentType = Node;
        return;
    }
    if (compenentTypeName == "Edge") {
        m_componentType = Edge;
        return;
    }
    if (compenentTypeName == "KernelModule") {
        m_componentType = KernelModule;
        return;
    }

    // else
    qCritical() << "Unable to understand componentType " << compenentTypeName;
    m_componentType = KernelModule;
}

Object::ComponentType Object::componentType() const
{
    return m_componentType;
}

void Object::setTitle(const QString &title)
{
    m_title = title;
}

QString Object::title() const
{
    return m_title;
}

void Object::setDescription(const QStringList &description)
{
    m_description = description;
}

QVariantList Object::description() const
{
    QVariantList list;
    for (const QString &paragraph : m_description) {
        list << paragraph;
    }
    return list;
}

void Object::setSyntaxExample(const QString &syntaxExample)
{
    m_syntaxExample = syntaxExample;
}

QString Object::syntaxExample() const
{
    return m_syntaxExample;
}

void Object::addProperty(Property *property)
{
    m_properties.append(property);
}

QList<Property *> Object::properties() const
{
    return m_properties;
}

void Object::addMethod(Method *method)
{
    m_methods.append(method);
}

QList<Method *> Object::methods() const
{
    return m_methods;
}

void Object::setObjectParent(const QString &id)
{
    m_objectParent = id;
}

QString Object::objectParent() const
{
    return m_objectParent;
}

QString Object::apiDocumentIdentifier()
{
    return m_id;
}
