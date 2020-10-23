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
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "editorplugins/editorplugininterface.h"
#include "logging_p.h"


using namespace GraphTheory;

class GraphTheory::EditorPluginInterfacePrivate
{
public:
    EditorPluginInterfacePrivate(const QString &componentName)
        : m_componentName(componentName)
    {

    }

    const QString m_componentName;
    QString m_displayName;
    QString m_lastErrorString;
};

EditorPluginInterface::EditorPluginInterface(const QString &componentName, QObject *parent)
    : QObject(parent)
    , d(new EditorPluginInterfacePrivate(componentName))
{

}

EditorPluginInterface::~EditorPluginInterface()
{

}

QString EditorPluginInterface::displayName()
{
    if (d->m_displayName.isEmpty()) {
        return d->m_componentName;
    }
    return d->m_displayName;
}

void EditorPluginInterface::setDisplayName(const QString &name)
{
    d->m_displayName = name;
}
