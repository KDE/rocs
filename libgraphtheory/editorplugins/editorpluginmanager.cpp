/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
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
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "editorpluginmanager.h"
#include "editorplugininterface.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

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

static QStringList readStringList(const QJsonObject &obj, const QString &key)
{
    const auto value = obj[key];
    if (value.isString()) {
        return QStringList(value.toString());
    }
    QStringList ret;
    if (value.isArray()) {
        foreach (const QJsonValue& i, value.toArray()) {
            ret << i.toString();
        }
    }
    return ret;
}

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
    QPluginLoader loader;
    foreach (const QString &dir, dirsToCheck) {
        QDirIterator it(dir, QDir::Files);
        qDebug() << "iterating over directory " << dir;
        while (it.hasNext()) {
            it.next();
            loader.setFileName(it.fileInfo().absoluteFilePath());
            QJsonObject m_metaData = loader.metaData()["MetaData"].toObject();
            if (!readStringList(m_metaData, "X-KDE-ServiceTypes").contains("rocs/editorplugins")) {
                continue;
            }
            qDebug() << "Load Plugin: " << m_metaData["Name"].toString();
            if (!loader.load()) {
                qCritical() << "Error while loading plugin: " << m_metaData["Name"].toString();
            }

            KPluginFactory *factory = KPluginLoader(loader.fileName()).factory();
            EditorPluginInterface *plugin = factory->create<EditorPluginInterface>(this);
            plugin->setDisplayName(m_metaData["Name"].toString());
            d->m_plugins.append(plugin);
        }
    }
}
