/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nodeitem.h"

using namespace GraphTheory;

class GraphTheory::NodeItemPrivate {
public:
    NodeItemPrivate()
        : m_color(Qt::black)
    {
    }

    ~NodeItemPrivate()
    {
    }

    QColor m_color;
};

NodeItem::NodeItem(QQuickItem *parent)
    : QQuickItem(parent)
    , d(new NodeItemPrivate)
{
    setFlag(QQuickItem::ItemHasContents, true);
}

NodeItem::~NodeItem()
{

}

QColor NodeItem::color() const
{
    return d->m_color;
}

void NodeItem::setColor(const QColor &color)
{
    if (color == d->m_color) {
        return;
    }
    d->m_color = color;
    emit colorChanged();
}
