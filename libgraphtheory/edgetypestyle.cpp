/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "edgetypestyle.h"
#include <QColor>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::EdgeTypeStylePrivate
{
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
    connect(this, &EdgeTypeStyle::colorChanged, this, &EdgeTypeStyle::changed);
    connect(this, &EdgeTypeStyle::visibilityChanged, this, &EdgeTypeStyle::changed);
    connect(this, &EdgeTypeStyle::propertyNamesVisibilityChanged, this, &EdgeTypeStyle::changed);
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
    Q_EMIT colorChanged(color);
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
    Q_EMIT visibilityChanged(visible);
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
    Q_EMIT propertyNamesVisibilityChanged(visible);
}

bool EdgeTypeStyle::isPropertyNamesVisible() const
{
    return d->m_propertyNamesVisible;
}
