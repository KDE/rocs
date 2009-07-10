/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#include "graphGroups.h"
#include "node.h"

GraphGroup::GraphGroup() : QObject(0), QList< ::Node*>() {}

GraphGroup::GraphGroup(const GraphGroup& g) : QObject(0), QList< ::Node*>() {
    foreach( ::Node *n, g) {
        append(n);
    }
    _name = g.name();
}
QString GraphGroup::name() const {
    return _name;
}

void GraphGroup::setName(const QString& name)  {
    _name = name;
}
