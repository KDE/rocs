/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorpluginmanager.h"
#include "logging_p.h"

#include <KPluginMetaData>
#include <KPluginFactory>
#include <QString>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>

using namespace GraphTheory;

class GraphTheory::EditorPluginManagerPrivate
{
public:
    EditorPluginManagerPrivate()
    {

    }

    ~EditorPluginManagerPrivate()
    { }

    QList<EditorPluginInterface*> m_plugins;
};

EditorPluginManager::EditorPluginManager()
    : d(new EditorPluginManagerPrivate)
{
    loadPlugins();
}

EditorPluginManager::~EditorPluginManager()
{

}

QList<EditorPluginInterface*> EditorPluginManager::plugins() const
{
    return d->m_plugins;
}

void EditorPluginManager::loadPlugins()
{
    // remove all present plugins
    qDeleteAll(d->m_plugins);
    d->m_plugins.clear();

    // load plugins
    const QVector<KPluginMetaData> metadataList = KPluginMetaData::findPlugins(QStringLiteral("rocs/editorplugins"));
    for (const auto &metadata : metadataList) {
        const auto result = KPluginFactory::instantiatePlugin<EditorPluginInterface>(metadata, this);

        if (!result) {
            qCWarning(GRAPHTHEORY_GENERAL) << "Failed to load editor plugin" << result.errorText;
            continue;
        }

        result.plugin->setDisplayName(metadata.name());
        d->m_plugins.append(result.plugin);
        qCDebug(GRAPHTHEORY_GENERAL) << "Loaded plugin:" << metadata.name();
    }
}
