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

#include <qcompilerdetection.h>
#include "Data.h"
#include <boost/weak_ptr.hpp>
#include <KLocale>
#include <QDebug>

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


GroupPtr Group::create(DataStructurePtr dataStructure, int uniqueIdentifier, int groupType)
{
    DataPtr data = Data::create<Group>(dataStructure, uniqueIdentifier, groupType);
    return boost::static_pointer_cast<Group>(data);
}


Group::Group(DataStructurePtr dataStructure, int identifier, int type):
    Data(dataStructure, identifier, type),
    d(new GroupPrivate())
{
    d->_dataStructure = dataStructure;
    d->_identifier = identifier;
    d->_name = i18n("Group");
}


Group::~Group()
{

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


DataList Group::dataList() const
{
    return d->_dataList;
}


void Group::clear()
{
    d->_dataList.clear();
    emit groupElementsChanged();
}


void Group::setName(const QString& name)
{
    d->_name = name;
}


QString Group::name() const
{
    return d->_name;
}
