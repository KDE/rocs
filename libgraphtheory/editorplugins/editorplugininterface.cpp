/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
