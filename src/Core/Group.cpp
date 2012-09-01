/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "Group.h"

#include <boost/weak_ptr.hpp>

class GroupPrivate
{
public:
    GroupPrivate() {}
    boost::weak_ptr<Group> q; // self pointer

    QString _name;
    int _identifier;
    DataList _dataList;
    DataStructurePtr _dataStructure;
};


GroupPtr Group::create(DataStructurePtr dataStructure, int uniqueIdentifier)
{
    GroupPtr pi(new Group(dataStructure, uniqueIdentifier));
    pi->d->q = pi;
    return pi;
}


Group::Group(DataStructurePtr dataStructure, int identifier):
    d(new GroupPrivate())
{
    d->_dataStructure = dataStructure;
    d->_identifier = identifier;
    d->_name = i18n("Group");
}


void Group::addData(DataPtr data)
{
    d->_dataList.append(data);
    emit groupElementsChanged();
}


void Group::addData(DataList dataList)
{
    d->_dataList.append(dataList);
    emit groupElementsChanged();
}


void Group::clear()
{
    d->_dataList.clear();
    emit groupElementsChanged();
}


void Group::setName(QString name)
{
    d->_name = name;
}


const QString& Group::name() const
{
    return d->_name;
}
