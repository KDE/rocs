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

#include "MethodDocumentation.h"
#include "ParameterDocumentation.h"

MethodDocumentation::MethodDocumentation(QObject* parent)
    : QObject(parent)
{

}

void MethodDocumentation::setName(const QString &name)
{
    _name = name;
}

QString MethodDocumentation::name() const
{
    return _name;
}

void MethodDocumentation::setDescription(const QStringList &description)
{
    _description = description;
}

QVariantList MethodDocumentation::description() const
{
    QVariantList list;
    foreach (QString paragraph, _description) {
        list << paragraph;
    }
    return list;
}

void MethodDocumentation::setReturnType(const QString& type)
{
    _returnType = type;
}

QString MethodDocumentation::returnType() const
{
    return _returnType;
}

void MethodDocumentation::addParameter(const QString& name, const QString& type, const QString& info)
{
    ParameterDocumentation *parameter = new ParameterDocumentation(this);
    parameter->setName(name);
    parameter->setType(type);
    parameter->setInfo(info);
    _parameters.append(parameter);
}

QVariant MethodDocumentation::parametersVar() const
{
    return QVariant::fromValue(_parameters);
}

QList< ParameterDocumentation* > MethodDocumentation::parameters() const
{
    return _parameters;
}

QString MethodDocumentation::apiDocumentAnchor()
{
    return QString("method").append(_name);
}
