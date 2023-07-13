/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#include "parameter.h"

Parameter::Parameter(QObject *parent)
    : QObject(parent)
{
}

QString Parameter::name() const
{
    return m_name;
}

void Parameter::setName(const QString &name)
{
    m_name = name;
}

QString Parameter::info() const
{
    return m_info;
}

void Parameter::setInfo(const QString &info)
{
    m_info = info;
}

QString Parameter::type() const
{
    return m_type;
}

void Parameter::setType(const QString &type)
{
    m_type = type;
}

void Parameter::setTypeLink(const QString &link)
{
    m_typeLink = link;
}

QString Parameter::typeLink() const
{
    return m_typeLink;
}

#include "moc_parameter.cpp"
