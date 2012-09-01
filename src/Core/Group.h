/*
    This file is part of Rocs.
    Copyright 2009-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef GROUP_H
#define GROUP_H

#include <DataStructure.h>

#include <boost/shared_ptr.hpp>

class GroupPrivate;

class Group : QObject
{
    Q_OBJECT
public:
    static GroupPtr create(DataStructurePtr dataStructure, int uniqueIdentifier);

    /**
     * Add data element to group.
     * \param data is data element that shall be added to this group
     */
    void addData(DataPtr data);

    /**
     * Add data element list to group;
     * \param dataList is list of data elements that shall be added to this group
     */
    void addData(DataList dataList);

    /**
     * Remove data element from this group.
     * \param data is data element that shall be removed
     */
    void removeData(DataPtr data);

    /**
     * Clear list of data elements belonging to this group.
     */
    void clear();

    /**
     * Name of this group.
     */
    const QString& name() const;

    void setName(QString name);

signals:
    void groupElementsChanged();

protected:
    /** Default constructor. To create Group elements use \see Group::create(...).
     *\param parent is the parent DataStructure
     *\param identifier is the unique identifier for this group
     */
    Group(DataStructurePtr dataStructure, int identifier);

private:
    boost::shared_ptr<GroupPrivate> const d;
};

#endif
