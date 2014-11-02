/*
    This file is part of Rocs.
    Copyright 2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PropertyDocumentation.h"

PropertyDocumentation::PropertyDocumentation(QObject* parent)
    : QObject(parent)
{

}

void PropertyDocumentation::setName(const QString &name)
{
    _name = name;
}

QString PropertyDocumentation::name() const
{
    return _name;
}

void PropertyDocumentation::setDescription(const QStringList &description)
{
    _description = description;
}

QVariantList PropertyDocumentation::description() const
{
    QVariantList list;
    foreach (QString paragraph, _description) {
        list << paragraph;
    }
    return list;
}

void PropertyDocumentation::setType(const QString &type)
{
    _type = type;
}

QString PropertyDocumentation::type() const
{
    return _type;
}

void PropertyDocumentation::setTypeLink(const QString& link)
{
    _typeLink = link;
}

QString PropertyDocumentation::typeLink() const
{
    return _typeLink;
}

QString PropertyDocumentation::apiDocumentAnchor()
{
    return QString("property").append(_name);
}
