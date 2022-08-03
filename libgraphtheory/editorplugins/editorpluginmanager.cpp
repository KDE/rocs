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
    QList<EditorPluginInterface*> m_plugins;
};

EditorPluginManager::EditorPluginManager()
    : d(new EditorPluginManagerPrivate)
{
    // load plugins
    const QVector<KPluginMetaData> metadataList = KPluginMetaData::findPlugins(QStringLiteral("rocs/editorplugins"));
    for (const auto &metadata : metadataList) {
        const auto result = KPluginFactory::instantiatePlugin<EditorPluginInterface>(metadata, this);

        if (!result) {
            qCWarning(GRAPHTHEORY_GENERAL) << "Failed to load editor plugin" << result.errorText;
            continue;
        }

        d->m_plugins.append(result.plugin);
        qCDebug(GRAPHTHEORY_GENERAL) << "Loaded plugin:" << metadata.name();
    }
}

EditorPluginManager::~EditorPluginManager() = default;

QList<EditorPluginInterface *> EditorPluginManager::plugins() const
{
    return d->m_plugins;
}
