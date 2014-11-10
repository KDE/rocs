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

#include "object.h"
#include <QStringList>
#include <QDebug>

Object::Object(QObject* parent)
    : QObject(parent)
{
}

Object::~Object()
{
}

void Object::setId(const QString &id)
{
    _id = id;
}

QString Object::id() const
{
    return _id;
}

void Object::setComponentType(const QString& compenentTypeName)
{
    if (compenentTypeName == "DataStructure") {
        _componentType = DataStructure;
        return;
    }
    if (compenentTypeName == "Data") {
        _componentType = Data;
        return;
    }
    if (compenentTypeName == "Pointer") {
        _componentType = Pointer;
        return;
    }
    if (compenentTypeName == "EngineComponent") {
        _componentType = EngineComponent;
        return;
    }

    // else
    qCritical() << "Unable to understand componentType " << compenentTypeName;
    _componentType = EngineComponent;
}

Object::ComponentType Object::componentType() const
{
    return _componentType;
}

void Object::setTitle(const QString &title)
{
    _title = title;
}

QString Object::title() const
{
    return _title;
}

void Object::setDescription(const QStringList &description)
{
    _description = description;
}

QVariantList Object::description() const
{
    QVariantList list;
    foreach (QString paragraph, _description) {
        list << paragraph;
    }
    return list;
}

void Object::setSyntaxExample(const QString &syntaxExample)
{
    _syntaxExample = syntaxExample;
}

QString Object::syntaxExample() const
{
    return _syntaxExample;
}

void Object::addProperty(Property* property)
{
    _properties.append(property);
}

QList< Property* > Object::properties() const
{
    return _properties;
}

void Object::addMethod(Method* method)
{
    _methods.append(method);
}

QList< Method* > Object::methods() const
{
    return _methods;
}

void Object::setObjectParent(const QString& id)
{
    _objectParent = id;
}

QString Object::objectParent() const
{
    return _objectParent;
}

QString Object::apiDocumentIdentifier()
{
    return _id;
}
