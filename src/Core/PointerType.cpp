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
#include <boost/weak_ptr.hpp>


class PointerTypePrivate{
public:
    PointerTypePrivate(){}
    boost::weak_ptr<PointerType> q; // self pointer

    QString _name;
    int _identifier;
    QColor _defaultColor;
    bool _valueVisibility;
    bool _nameVisibility;
    DataStructurePtr _dataStructure;
};


PointerTypePtr PointerType::create(DataStructurePtr dataStructure, int identifier) {
    PointerTypePtr pi(new PointerType(dataStructure, identifier));
    pi->d->q = pi;

    return pi;
}


PointerType::PointerType(DataStructurePtr dataStructure, int identifier):
    d(new PointerTypePrivate())
{
    d->_identifier = identifier;
    d->_defaultColor = QColor("gray");
    d->_nameVisibility = false;
    d->_valueVisibility = true;
    d->_dataStructure = dataStructure;
}


PointerType::~PointerType()
{
}


void PointerType::setName(QString name)
{
    d->_name = name;
}


QString PointerType::name() const
{
    return d->_name;
}

int PointerType::identifier() const
{
    return d->_identifier;
}


void PointerType::setNameVisible(bool visible)
{
    d->_nameVisibility = visible;
    d->_dataStructure->setPointerNameVisibility(visible, d->_identifier);
}


bool PointerType::isNameVisible() const
{
    return d->_nameVisibility;
}


void PointerType::setValueVisible(bool visible)
{
    d->_valueVisibility = visible;
    d->_dataStructure->setPointerValueVisibility(visible, d->_identifier);
}


bool PointerType::isValueVisible() const
{
    return d->_valueVisibility;
}


void PointerType::setDefaultColor(QColor color)
{
    d->_defaultColor = color;
}


QColor PointerType::defaultColor() const
{
    return d->_defaultColor;
}
