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
    foreach(EditorPluginInterface *f, d->m_plugins) {
        delete f;
    }
    d->m_plugins.clear();

    // dirs to check for plugins
    QStringList dirsToCheck;
    foreach (const QString &directory, QCoreApplication::libraryPaths()) {
        dirsToCheck << directory + QDir::separator() + "rocs/editorplugins";
    }

    // load plugins
    foreach (const QString &dir, dirsToCheck) {
        QVector<KPluginMetaData> metadataList = KPluginLoader::findPlugins(dir,[=](const KPluginMetaData &data){
            return data.serviceTypes().contains("rocs/editorplugins");
        });
        for (const auto &metadata : metadataList) {
            KPluginFactory *factory = KPluginLoader(metadata.fileName()).factory();
            EditorPluginInterface *plugin = factory->create<EditorPluginInterface>(this);
            plugin->setDisplayName(metadata.name());
            d->m_plugins.append(plugin);
            qCDebug(GRAPHTHEORY_GENERAL) << "Loaded plugin:" << metadata.name();
        }
    }
}
