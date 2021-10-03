/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "nodetypestyle.h"
#include <QColor>
#include <QString>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::NodeTypeStylePrivate {
public:
    NodeTypeStylePrivate()
        : m_color(77, 77, 77) // dark gray
        , m_visible(true)
        , m_propertyNamesVisible(false)
    {
    }

    ~NodeTypeStylePrivate()
    {
    }

    QColor m_color;
    bool m_visible;
    bool m_propertyNamesVisible;
};

NodeTypeStyle::NodeTypeStyle()
    : QObject()
    , d(new NodeTypeStylePrivate)
{
    connect(this, &NodeTypeStyle::colorChanged,
        this, &NodeTypeStyle::changed);
    connect(this, &NodeTypeStyle::visibilityChanged,
        this, &NodeTypeStyle::changed);
    connect(this, &NodeTypeStyle::propertyNamesVisibilityChanged,
        this, &NodeTypeStyle::changed);
}

NodeTypeStyle::~NodeTypeStyle()
{

}

void NodeTypeStyle::setColor(const QColor &color)
{
    if (d->m_color == color) {
        return;
    }
    d->m_color = color;
    Q_EMIT colorChanged(color);
}

QColor NodeTypeStyle::color() const
{
    return d->m_color;
}

void NodeTypeStyle::setVisible(bool visible)
{
    if (d->m_visible == visible) {
        return;
    }
    d->m_visible = visible;
    Q_EMIT visibilityChanged(visible);
}

bool NodeTypeStyle::isVisible() const
{
    return d->m_visible;
}

void NodeTypeStyle::setPropertyNamesVisible(bool visible)
{
    if (d->m_propertyNamesVisible == visible) {
        return;
    }
    d->m_propertyNamesVisible = visible;
    Q_EMIT propertyNamesVisibilityChanged(visible);
}

bool NodeTypeStyle::isPropertyNamesVisible() const
{
    return d->m_propertyNamesVisible;
}
