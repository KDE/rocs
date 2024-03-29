/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "editorpluginmanager.h"
#include "logging_p.h"

#include <KPluginFactory>
#include <KPluginMetaData>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

using namespace GraphTheory;

class GraphTheory::EditorPluginManagerPrivate
{
public:
    QList<EditorPluginInterface *> m_plugins;
};

EditorPluginManager::EditorPluginManager()
    : d(new EditorPluginManagerPrivate)
{
    // load plugins
    const QVector<KPluginMetaData> metadataList = KPluginMetaData::findPlugins(QStringLiteral("rocs/editorplugins"));
    for (const auto &metadata : metadataList) {
        const auto result = KPluginFactory::instantiatePlugin<EditorPluginInterface>(metadata, this);

        if (result) {
            d->m_plugins.append(result.plugin);
            qCDebug(GRAPHTHEORY_GENERAL) << "Loaded editor plugin:" << metadata.pluginId();
        } else {
            qCWarning(GRAPHTHEORY_GENERAL) << "Failed to load editor plugin" << result.errorText;
        }
    }
}

EditorPluginManager::~EditorPluginManager() = default;

QList<EditorPluginInterface *> EditorPluginManager::plugins() const
{
    return d->m_plugins;
}

#include "moc_editorpluginmanager.cpp"
