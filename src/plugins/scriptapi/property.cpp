/*
 *  Copyright 2013-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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

#include "property.h"
#include <QVariant>

Property::Property(QObject* parent)
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
    foreach (QString paragraph, m_description) {
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

void Property::setTypeLink(const QString& link)
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
