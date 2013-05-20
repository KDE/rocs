/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#ifndef LISTNODE_H
#define LISTNODE_H

#include "rocs_liststructure_export.h"

#include <Data.h>
#include <boost/shared_ptr.hpp>


class ROCS_LISTSTRUCTURE_EXPORT ListNode : public Data
{
    Q_OBJECT
    Q_PROPERTY(QScriptValue next READ nextNodeScriptValue)

public:
    static DataPtr create(DataStructurePtr parent, int uniqueIdentifier, int dataType);
    ListNode(DataStructurePtr parent, int uniqueIdentifier, int dataType);
    ~ListNode();

    /**
     * Return target of out-pointer if exactly one outgoing pointer exists, otherwise returns empty node.
     * \return next node
     */
    boost::shared_ptr<ListNode> next() const;
    QScriptValue nextNodeScriptValue();

    /**
     * Set outgoing pointer to \p target.
     */
    Q_INVOKABLE void pointTo(boost::shared_ptr<ListNode> target);
};

#endif // LISTNODE_H
