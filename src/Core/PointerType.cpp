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
#include <boost/weak_ptr.hpp>


class PointerTypePrivate
{
public:
    PointerTypePrivate() {}
    boost::weak_ptr<PointerType> q; // self pointer

    QString _name;
    int _identifier;
    PointerType::Direction _pointerDirection;
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


PointerType::PointerType(Document* document, int identifier):
    d(new PointerTypePrivate())
{
    d->_identifier = identifier;
    d->_pointerDirection = Unidirectional;
    d->_defaultColor = QColor("gray");
    d->_name = i18n("Connection");
    d->_document = document;
}


PointerType::~PointerType()
{
}


void PointerType::setName(QString name)
{
    d->_name = name;
    emit(nameChanged(d->_name));
}


const QString& PointerType::name() const
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


void PointerType::setDefaultColor(QColor color)
{
    d->_defaultColor = color;
    emit(colorChanged(d->_defaultColor));
}


const QColor& PointerType::defaultColor() const
{
    return d->_defaultColor;
}
