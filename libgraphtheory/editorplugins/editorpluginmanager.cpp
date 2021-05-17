/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorpluginmanager.h"
#include "logging_p.h"

#include <KPluginMetaData>
#include <KPluginLoader>
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
    const QVector<KPluginMetaData> metadataList = KPluginLoader::findPlugins(QStringLiteral("rocs/editorplugins"));
    for (const auto &metadata : metadataList) {
        KPluginFactory *factory = KPluginLoader(metadata.fileName()).factory();
        EditorPluginInterface *plugin = factory->create<EditorPluginInterface>(this);
        plugin->setDisplayName(metadata.name());
        d->m_plugins.append(plugin);
        qCDebug(GRAPHTHEORY_GENERAL) << "Loaded plugin:" << metadata.name();
    }
}
