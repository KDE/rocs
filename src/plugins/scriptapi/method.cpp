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
    foreach (QString paragraph, m_description) {
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
