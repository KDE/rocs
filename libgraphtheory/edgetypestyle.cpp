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

#include "edgetypestyle.h"
#include <QColor>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::EdgeTypeStylePrivate {
public:
    EdgeTypeStylePrivate()
        : m_color(77, 77, 77) // dark gray
        , m_visible(true)
        , m_propertyNamesVisible(false)
    {
    }

    ~EdgeTypeStylePrivate()
    {
    }

    QColor m_color;
    bool m_visible;
    bool m_propertyNamesVisible;
};

EdgeTypeStyle::EdgeTypeStyle()
    : QObject()
    , d(new EdgeTypeStylePrivate)
{

}

EdgeTypeStyle::~EdgeTypeStyle()
{

}

void EdgeTypeStyle::setColor(const QColor &color)
{
    if (d->m_color == color) {
        return;
    }
    d->m_color = color;
    emit colorChanged(color);
}

QColor EdgeTypeStyle::color() const
{
    return d->m_color;
}

void EdgeTypeStyle::setVisible(bool visible)
{
    if (d->m_visible == visible) {
        return;
    }
    d->m_visible = visible;
    emit visibilityChanged(visible);
}

bool EdgeTypeStyle::isVisible() const
{
    return d->m_visible;
}

void EdgeTypeStyle::setPropertyNamesVisible(bool visible)
{
    if (d->m_propertyNamesVisible == visible) {
        return;
    }
    d->m_propertyNamesVisible = visible;
    emit propertyNamesVisibilityChanged(visible);
}

bool EdgeTypeStyle::isPropertyNamesVisible() const
{
    return d->m_propertyNamesVisible;
}
