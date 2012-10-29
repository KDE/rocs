/*
    This file is part of Rocs.
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "PointerType.h"
#include "DataStructure.h"
#include "Pointer.h"
#include <KDebug>
#include <boost/weak_ptr.hpp>


class PointerTypePrivate
{
public:
    struct Property {
        QString name;
        QVariant defaultValue;
        bool visible;
    };

    PointerTypePrivate(Document* document, int identifier) :
        _name(i18n("Connection"))
        , _identifier(identifier)
        , _pointerDirection(PointerType::Unidirectional)
        , _lineStyle(Qt::SolidLine)
        , _defaultColor(QColor("gray"))
        , _document(document)
    {}
    boost::weak_ptr<PointerType> q; // self pointer

    QMap<QString, Property> _propertyList;
    QList<QString> _propertyDisplayList;
    QString _name;
    int _identifier;
    PointerType::Direction _pointerDirection;
    Qt::PenStyle _lineStyle;
    QColor _defaultColor;
    bool _valueVisibility;
    bool _nameVisibility;
    bool _visibility;
    Document* _document;
};


PointerTypePtr PointerType::create(Document* document, int identifier)
{
    PointerTypePtr pi(new PointerType(document, identifier));
    pi->d->q = pi;

    return pi;
}


PointerType::PointerType(Document* document, int identifier)
    : d(new PointerTypePrivate(document, identifier))
{
}


PointerType::~PointerType()
{
}


void PointerType::setName(const QString& name)
{
    d->_name = name;
    emit(nameChanged(d->_name));
}


QString PointerType::name() const
{
    return d->_name;
}


int PointerType::identifier() const
{
    return d->_identifier;
}


PointerType::Direction PointerType::direction() const
{
    return d->_pointerDirection;
}


void PointerType::setDirection(PointerType::Direction direction)
{
    d->_pointerDirection = direction;
    emit(directionChanged(direction));
}


Qt::PenStyle PointerType::lineStyle() const
{
    return d->_lineStyle;
}


void PointerType::setLineStyle(Qt::PenStyle lineStyle)
{
    d->_lineStyle = lineStyle;
    emit(styleChanged());
}


void PointerType::setDefaultColor(const QColor& color)
{
    d->_defaultColor = color;
    emit(styleChanged());
}


QColor PointerType::defaultColor() const
{
    return d->_defaultColor;
}


void PointerType::addProperty(const QString& name, const QString& defaultValue)
{
    PointerTypePrivate::Property newProperty;
    newProperty.name = name;
    newProperty.defaultValue = defaultValue;
    newProperty.visible = true;
    d->_propertyList.insert(name, newProperty);
    d->_propertyDisplayList.append(name);
    emit(propertyAdded(newProperty.name, newProperty.defaultValue));
}

void PointerType::removeProperty(const QString& name)
{
    d->_propertyDisplayList.removeOne(name);
    d->_propertyList.remove(name);
    emit propertyRemoved(name);
}

void PointerType::renameProperty(const QString& oldName, const QString& newName)
{
    if (d->_propertyList.contains(newName)) {
        kWarning() << "Property name was not changed: new name already exists";
        return;
    }
    int index = d->_propertyDisplayList.indexOf(oldName);
    if (index >= 0) {
        d->_propertyDisplayList.removeAt(index);
        d->_propertyDisplayList.insert(index, newName);
    }
    d->_propertyList.insert(newName, d->_propertyList[oldName]);
    d->_propertyList.remove(oldName);
    emit propertyRenamed(oldName, newName);
}

QList<QString> PointerType::properties() const
{
    return d->_propertyList.keys();
}

QVariant PointerType::propertyDefaultValue(const QString& name) const
{
    if (!d->_propertyList.contains(name)) {
        return false;
    }
    return d->_propertyList[name].defaultValue;
}

void PointerType::setPropertyDefaultValue(const QString& name, const QVariant& value)
{
    if (!d->_propertyList.contains(name)) {
        return;
    }
    d->_propertyList[name].defaultValue = value;
    emit propertyDefaultValueChanged(name);
}

bool PointerType::isPropertyVisible(const QString& name) const
{
    if (!d->_propertyList.contains(name)) {
        return false;
    }
    return d->_propertyList[name].visible;
}

void PointerType::setPropertyVisible(const QString& name, bool visible)
{
    if (!d->_propertyList.contains(name)) {
        return;
    }
    d->_propertyList[name].visible = visible;
    emit propertyVisibilityChanged(name);
}

void PointerType::remove()
{
    emit removed();
    disconnect();
}
