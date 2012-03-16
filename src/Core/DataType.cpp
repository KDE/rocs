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


#include "DataType.h"
#include "DataStructure.h"
#include <boost/weak_ptr.hpp>


class DataTypePrivate{
public:
    DataTypePrivate(){}
    boost::weak_ptr<DataType> q; // self pointer

    QString _name;
    QColor _defaultColor;
    int _identifier;
    bool _valueVisibility;
    bool _nameVisibility;
    bool _visibility;
    DataStructurePtr _dataStructure;
};


DataTypePtr DataType::create(DataStructurePtr dataStructure, int identifier) {
    DataTypePtr pi(new DataType(dataStructure, identifier));
    pi->d->q = pi;

    return pi;
}


DataType::DataType(DataStructurePtr dataStructure, int identifier):
    d(new DataTypePrivate())
{
    d->_identifier = identifier;
    d->_defaultColor = QColor("blue");
    d->_nameVisibility = true;
    d->_valueVisibility = true;
    d->_visibility = true;
    d->_dataStructure = dataStructure;
}


DataType::~DataType()
{
}

int DataType::identifier() const
{
    return d->_identifier;
}

void DataType::setName(QString name)
{
    d->_name = name;
}


QString DataType::name() const
{
    return d->_name;
}


void DataType::setNameVisible(bool visible)
{
    d->_nameVisibility = visible;
    d->_dataStructure->setDataNameVisibility(visible, d->_identifier);
}


bool DataType::isNameVisible() const
{
    return d->_nameVisibility;
}


void DataType::setValueVisible(bool visible)
{
    d->_valueVisibility = visible;
    d->_dataStructure->setDataValueVisibility(visible, d->_identifier);
}


bool DataType::isValueVisible() const
{
    return d->_valueVisibility;
}


void DataType::setVisible(bool visible)
{
    d->_visibility = visible;
    d->_dataStructure->setDataVisibility(visible, d->_identifier);
}


bool DataType::isVisible() const
{
    return d->_visibility;
}

void DataType::setDefaultColor(QColor color)
{
    d->_defaultColor = color;
}


QColor DataType::defaultColor() const
{
    return d->_defaultColor;
}
