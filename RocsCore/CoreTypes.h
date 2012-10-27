/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#ifndef CORETYPES_H
#define CORETYPES_H

#include <QList>
#include <boost/shared_ptr.hpp>

class Data;
class Pointer;
class DataType;
class PointerType;
class DataStructure;
class Group;

typedef QList< boost::shared_ptr<Data> > DataList;
typedef QList< boost::shared_ptr<Pointer> > PointerList;
typedef boost::shared_ptr<Data> DataPtr;
typedef boost::shared_ptr<Pointer> PointerPtr;
typedef boost::shared_ptr<DataType> DataTypePtr;
typedef boost::shared_ptr<PointerType> PointerTypePtr;
typedef boost::shared_ptr<DataStructure> DataStructurePtr;
typedef boost::shared_ptr<Group> GroupPtr;

#endif
