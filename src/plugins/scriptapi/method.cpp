/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "method.h"
#include "parameter.h"

Method::Method(QObject* parent)
    : QObject(parent)
{

}

void Method::setName(const QString &name)
{
    m_name = name;
}

QString Method::name() const
{
    return m_name;
}

void Method::setDescription(const QStringList &description)
{
    m_description = description;
}

QVariantList Method::description() const
{
    QVariantList list;
    for (const QString &paragraph : m_description) {
        list << paragraph;
    }
    return list;
}

void Method::setReturnType(const QString& type)
{
    m_returnType = type;
}

QString Method::returnType() const
{
    return m_returnType;
}

void Method::addParameter(const QString& name, const QString& type, const QString& info, const QString &typeLink = QString())
{
    Parameter *parameter = new Parameter(this);
    parameter->setName(name);
    parameter->setType(type);
    parameter->setInfo(info);
    parameter->setTypeLink(typeLink);
    m_parameters.append(parameter);
}

QVariant Method::parametersVar() const
{
    return QVariant::fromValue(m_parameters);
}

QList<Parameter*> Method::parameters() const
{
    return m_parameters;
}

QString Method::apiDocumentAnchor()
{
    return QString("method").append(m_name);
}

void Method::setReturnTypeLink(const QString& link)
{
    m_returnTypeLink = link;
}

QString Method::returnTypeLink() const
{
    return m_returnTypeLink;
}
