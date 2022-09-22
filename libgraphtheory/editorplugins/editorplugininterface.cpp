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
    EditorPluginInterfacePrivate(const KPluginMetaData &data)
        : m_componentName(data.pluginId())
        , m_displayName(data.name())
    {
    }

    const QString m_componentName;
    const QString m_displayName;
};

EditorPluginInterface::EditorPluginInterface(QObject *parent, const KPluginMetaData &data)
    : QObject(parent)
    , d(new EditorPluginInterfacePrivate(data))
{
}

EditorPluginInterface::~EditorPluginInterface() = default;

QString EditorPluginInterface::displayName() const
{
    if (d->m_displayName.isEmpty()) {
        return d->m_componentName;
    }
    return d->m_displayName;
}
