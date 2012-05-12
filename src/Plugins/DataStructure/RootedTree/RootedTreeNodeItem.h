/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef ROOTEDTREENODEITEM_H
#define ROOTEDTREENODEITEM_H

#include "DataItem.h"
#include <QVariant>
#include <QGraphicsScene>

class Data;

class RootedTreeNodeItem : public DataItem
{
    Q_OBJECT

public:
    RootedTreeNodeItem(DataPtr n);
    ~RootedTreeNodeItem();

    QRectF boundingRect() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);

};

#endif //ROOTEDTREENODEITEM_H
