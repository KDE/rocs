/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "GraphFileBackendManager.h"

#include "GraphFilePluginInterface.h"
#include "RocsGraphFileFormatPlugin.h"

#include <KServiceTypeTrader>
#include <KPluginInfo>
#include <QDebug>

class GraphFileBackendManagerPrivate
{
public:
    GraphFileBackendManagerPrivate() {
        backendInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/GraphFilePlugin"));
    }

    ~GraphFileBackendManagerPrivate()
    { }

    KPluginInfo::List backendInfo;
    QList<GraphFilePluginInterface*> backends;
    GraphFilePluginInterface* defaultGraphFilePlugin;
};


GraphFileBackendManager * GraphFileBackendManager::instance = 0;


GraphFileBackendManager * GraphFileBackendManager::self()
{
    if (GraphFileBackendManager::instance == 0) {
        GraphFileBackendManager::instance = new GraphFileBackendManager;
    }
    return GraphFileBackendManager::instance;
}


GraphFileBackendManager::GraphFileBackendManager()
{
    d = new GraphFileBackendManagerPrivate();
    loadBackends();
}


GraphFileBackendManager::~GraphFileBackendManager()
{
    delete d;
}


QList<GraphFilePluginInterface*> GraphFileBackendManager::backends() const
{
    return d->backends;
}


QList<GraphFilePluginInterface*> GraphFileBackendManager::backends(PluginType type) const
{
    QList<GraphFilePluginInterface*> backends;
    foreach(GraphFilePluginInterface* backend, d->backends) {
        switch(type) {
            case Import:
                if (backend->pluginCapability() == GraphFilePluginInterface::ImportOnly
                    || backend->pluginCapability() == GraphFilePluginInterface::ImportAndExport)
                {
                    backends.append(backend);
                }
                break;
            case Export:
                if (backend->pluginCapability() == GraphFilePluginInterface::ExportOnly
                    || backend->pluginCapability() == GraphFilePluginInterface::ImportAndExport)
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


void GraphFileBackendManager::loadBackends()
{
    // remove all present backends
    foreach(GraphFilePluginInterface * f, d->backends) {
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

        GraphFilePluginInterface *plugin = factory->create<GraphFilePluginInterface>(this);

        if (plugin) {
            d->backends.append(plugin);
        } else {
            qWarning() << "Could not load backend: " << service->name();
        }
    }

    // load static plugins
    GraphFilePluginInterface *plugin = new RocsGraphFileFormatPlugin(this);
    d->backends.append(plugin);
    d->defaultGraphFilePlugin = plugin;
}


GraphFilePluginInterface* GraphFileBackendManager::backendByExtension(QString ext)
{
    foreach(GraphFilePluginInterface * p,  d->backends) {
        if (p->extensions().join(";").contains(ext, Qt::CaseInsensitive)) {
            return p;
        }
    }
    return 0;
}


GraphFilePluginInterface* GraphFileBackendManager::defaultBackend()
{
    return d->defaultGraphFilePlugin;
}
