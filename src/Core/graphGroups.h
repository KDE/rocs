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


#ifndef GRAPHGROUPS_H
#define GRAPHGROUPS_H

#include <QObject>
#include <QList>
#include "rocslib_export.h"
class Node;

class  ROCSLIB_EXPORT GraphGroup : public QObject, public QList< ::Node*> {
    Q_OBJECT

public:
    GraphGroup();
    GraphGroup(const GraphGroup& g);

public slots:
    QString name() const;
    void setName(const QString&  name);

private:
    QString _name;
};
#endif
