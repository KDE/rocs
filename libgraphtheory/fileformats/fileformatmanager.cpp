/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "fileformatmanager.h"
#include "fileformatinterface.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::FileFormatManagerPrivate
{
public:
    FileFormatManagerPrivate() {
        backendInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/GraphFilePlugin"));
    }

    ~FileFormatManagerPrivate()
    { }

    KPluginInfo::List backendInfo;
    QList<FileFormatInterface*> backends;
    FileFormatInterface *defaultGraphFilePlugin;
};

FileFormatManager::FileFormatManager()
    : d(new FileFormatManagerPrivate)
{
    loadBackends();
}

FileFormatManager::~FileFormatManager()
{

}

QList<FileFormatInterface*> FileFormatManager::backends() const
{
    return d->backends;
}

QList<FileFormatInterface*> FileFormatManager::backends(PluginType type) const
{
    QList<FileFormatInterface*> backends;
    foreach(FileFormatInterface *backend, d->backends) {
        switch(type) {
            case Import:
                if (backend->pluginCapability() == FileFormatInterface::ImportOnly
                    || backend->pluginCapability() == FileFormatInterface::ImportAndExport)
                {
                    backends.append(backend);
                }
                break;
            case Export:
                if (backend->pluginCapability() == FileFormatInterface::ExportOnly
                    || backend->pluginCapability() == FileFormatInterface::ImportAndExport)
                {
                    backends.append(backend);
                }
                break;
            default:
                break;
        }
    }
    return backends;
}

void FileFormatManager::loadBackends()
{
    // remove all present backends
    foreach(FileFormatInterface *f, d->backends) {
        delete f;
    }
    d->backends.clear();

    // load dynamic backends
    KService::List offers = KServiceTypeTrader::self()->query("Rocs/GraphFilePlugin");
    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter) {
        KService::Ptr service = *iter;
        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory) {
            qCritical() << "KPluginFactory could not load the plugin: " << service->library();
            continue;
        }

        FileFormatInterface *plugin = factory->create<FileFormatInterface>(this);

        if (plugin) {
            d->backends.append(plugin);
        } else {
            qWarning() << "Could not load backend: " << service->name();
        }
    }

    // load static plugins
//     FileFormatInterface *plugin = new RocsGraphFileFormatPlugin(this); //TODO port to new plugin
//     d->backends.append(plugin);
//     d->defaultGraphFilePlugin = plugin;
}

FileFormatInterface * FileFormatManager::backendByExtension(const QString &ext)
{
    foreach(FileFormatInterface * p,  d->backends) {
        if (p->extensions().join(";").contains(ext, Qt::CaseInsensitive)) {
            return p;
        }
    }
    return 0;
}

FileFormatInterface * FileFormatManager::defaultBackend()
{
    return d->defaultGraphFilePlugin;
}
