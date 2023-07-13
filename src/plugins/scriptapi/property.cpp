/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "property.h"
#include <QVariant>

Property::Property(QObject *parent)
    : QObject(parent)
{
}

void Property::setName(const QString &name)
{
    m_name = name;
}

QString Property::name() const
{
    return m_name;
}

void Property::setDescription(const QStringList &description)
{
    m_description = description;
}

QVariantList Property::description() const
{
    QVariantList list;
    for (const QString &paragraph : m_description) {
        list.append(paragraph);
    }
    return list;
}

void Property::setType(const QString &type)
{
    m_type = type;
}

QString Property::type() const
{
    return m_type;
}

void Property::setTypeLink(const QString &link)
{
    m_typeLink = link;
}

QString Property::typeLink() const
{
    return m_typeLink;
}

QString Property::apiDocumentAnchor()
{
    return QString("property").append(m_name);
}

#include "moc_property.cpp"
