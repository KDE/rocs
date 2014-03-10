/*
    This file is part of Rocs.
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2013  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "DataStructureBackendManager.h"
#include "DataStructureBackendInterface.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <QDebug>
#include <KStandardDirs>
#include <KDE4Support/kglobal.h> //FIXME remove

#include <QHash>
#include "DataStructure.h"
#include "Data.h"
#include "Pointer.h"

/**
 * \class DataStructureBackendManagerPrivate
 * \internal
 *
 * This is the private data class for \see DataStructureBackendManager.
 * Note that -- even if the DataStructureBackendManager is contructed
 * before its first call, the plugins are only loaded after a first call
 * of lazyInit(), triggered by DataStructureBackendManager::self().
 */
class DataStructureBackendManagerPrivate
{
public:

    typedef KPluginInfo::List KPluginList;

    QObject *_parent;
    bool _initialized;
    KPluginList _pluginInfo;
    DataStructureBackendInterface *_currentPlugin;
    QHash<QString, DataStructureBackendInterface*> _pluginList;

    DataStructureBackendManagerPrivate(QObject *parent)
        : _parent(parent)
        , _initialized(false)
        , _pluginInfo(KPluginList())
        , _currentPlugin(0)
    {
    }

    ~DataStructureBackendManagerPrivate()
    {
        _currentPlugin = 0;

        // clear plugins
        qDeleteAll<QHash<QString, DataStructureBackendInterface*>::iterator>(_pluginList.begin(), _pluginList.end());
        _pluginList.clear();
    }

    void lazyInit()
    {
        if(!_initialized) {
            _initialized = true;
            loadBackends();
        }
    }

    DataStructurePtr convertDataStructureToActiveBackend(const DataStructurePtr dataStructure, Document* parent)
    {
        if (_currentPlugin) {
            return _currentPlugin->convertToDataStructure(dataStructure, parent);
        }
        return dataStructure;
    }

    KPluginInfo pluginInfo(DataStructureBackendInterface *plugin)
    {
        QString name = _pluginList.key(plugin);
        foreach(const KPluginInfo& info, _pluginInfo) {
            // we only want to compere the internal name and not the actual translated name
            QString identifier = info.property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString();
            if (identifier.isEmpty()) {
                identifier = info.name();
            }
            if (identifier == name) {
                return info;
            }
        }
        return KPluginInfo();
    }

    DataStructurePtr createDataStructure(Document *arg1 , const QString &pluginName)
    {
        if (!pluginName.isEmpty()) {
            if (DataStructureBackendInterface *plugin = _pluginList.value(pluginName)) {
                return plugin->createDataStructure(arg1);
            }
        } else if (_currentPlugin) {
            return _currentPlugin->createDataStructure(arg1);
        }
        return DataStructurePtr();
    }

    DataStructureBackendInterface * backend(const QString &pluginIdentifier)
    {
        if (_pluginList.contains(pluginIdentifier)) {
            return _pluginList[pluginIdentifier];
        }
        return 0;
    }

    void setActivePlugin(const QString &pluginIdentifier)
    {
        if (DataStructureBackendInterface *plg = backend(pluginIdentifier)) {
            _currentPlugin = plg;
        }
    }

    void loadBackends()
    {
        KServiceTypeTrader *trader = KServiceTypeTrader::self();
        QString serviceType = QString::fromLatin1("Rocs/DataStructurePlugin");

        // standard read of backend services
        KService::List offers = trader->query(serviceType);
        KPluginInfo::List offerInfos = KPluginInfo::fromServices(offers);

        // fallback to load default data structures "by hand"
        // this is the case when no kbuildsycoa4 run is performed and hence no trader cache was generated
        QStringList defaultBackends;
        defaultBackends << "rocs_GraphStructure.desktop"
            << "rocs_ListStructure.desktop"
            << "rocs_RootedTreeStructure.desktop";
        foreach (QString pluginDesktopFile, defaultBackends) {
            QString path = KGlobal::dirs()->findResource("services", pluginDesktopFile);
            if (path.isEmpty()) {
                qWarning() << "Could not find default plugin desktop file: " << pluginDesktopFile;
                continue;
            }

            // check if service is loaded from trader
            KService::Ptr service = QExplicitlySharedDataPointer<KService>(new KService(path));
            KPluginInfo *info = new KPluginInfo(service);
            bool found = false;
            foreach (KPluginInfo offerInfo, offerInfos) {
                if (info->property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString()
                    == offerInfo.property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString())
                {
                    found = true;
                    break;
                }
            }
            if (!found) {
                qWarning() << "Backend manager falls back to manually load structure"
                           << info->property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString()
                           << "The structure was not found in the service cache.";
                offers.append(service);
            }
            delete info;
        }

        // set up found backends
        _pluginInfo = KPluginInfo::fromServices(offers);
        foreach (KService::Ptr service, offers) {
            qDebug() << "set up backend " << service->library();
            QString error;
            KPluginLoader loader(service->library());
            KPluginFactory *factory = loader.factory();

            if (!factory) {
                qCritical() << "KPluginFactory could not load plugin"
                    << service->library()
                    << ", gave up with error message: "
                    << loader.errorString();
                continue;
            }

            KPluginInfo *info = new KPluginInfo(service);
            DataStructureBackendInterface *plugin = factory->create<DataStructureBackendInterface>(_parent);

            if (plugin) {
                KPluginInfo *info = new KPluginInfo(service);
                QString identifier = info->property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString();
                if (identifier.isEmpty()) {
                    identifier = info->name();
                    qWarning() << "No data structure backend identifier exists for plugin " << identifier << ", using its name.";
                }
                _pluginList.insert(identifier, plugin);
                info->setPluginEnabled(true);
                continue;
            } else {
                qCritical() << "Error while loading data structure backend \"" << info->name();
            }
        }

        // at least one data structure plugin is required
        if (_pluginList.isEmpty()) {
            qCritical() << "No backend found, cannot set active data structure backend.";
            return;
        }

        QHash<QString, DataStructureBackendInterface*>::const_iterator iter = _pluginList.constBegin();
        while (iter != _pluginList.constEnd()) {
            if (iter.value()->internalName() == QLatin1String("Graph")){
                _currentPlugin = iter.value();
                return;
            }
            ++iter;
        }

        // set last plugin, if "Graph" was not found
        _currentPlugin = _pluginList.values().last();
    }
};


DataStructureBackendManager::DataStructureBackendManager()
    : d(new DataStructureBackendManagerPrivate(this))
{
}

DataStructureBackendManager::~DataStructureBackendManager()
{
}

DataStructureBackendManager & DataStructureBackendManager::self()
{
    static DataStructureBackendManager instance;
    instance.d->lazyInit();
    return instance;
}

void DataStructureBackendManager::setBackend(const QString &pluginIdentifier)
{
    DataStructureBackendInterface* plugin = d->backend(pluginIdentifier);
    if (!plugin) {
        qCritical() << "Could not set \"" << pluginIdentifier << "\" as current backend, aborting.";
        return;
    }
    if (d->_currentPlugin && pluginIdentifier == d->_currentPlugin->internalName()) {
        return;
    }

    d->setActivePlugin(pluginIdentifier);
    emit backendChanged(pluginIdentifier);
}

DataStructurePtr DataStructureBackendManager::createDataStructure(const DataStructurePtr dataStructure, Document *parent)
{
    return d->convertDataStructureToActiveBackend(dataStructure, parent);
}

DataStructurePtr DataStructureBackendManager::createDataStructure(Document *parent, const QString &pluginName)
{
    return d->createDataStructure(parent, pluginName);
}

KPluginInfo DataStructureBackendManager::backendInfo(DataStructureBackendInterface *plugin) const
{
    return d->pluginInfo(plugin);
}

const QStringList DataStructureBackendManager::backends() const
{
    return d->_pluginList.keys();
}

DataStructureBackendInterface * DataStructureBackendManager::activeBackend() const
{
    return d->_currentPlugin;
}

DataStructureBackendInterface * DataStructureBackendManager::backend(const QString &internalName) const
{
    return d->backend(internalName);
}

QGraphicsItem * DataStructureBackendManager::dataItem(DataPtr data) const
{
    if (DataStructureBackendInterface *plg = data->dataStructure()->document()->backend()) {
        return plg->dataItem(data);
    }
    return d->_currentPlugin->dataItem(data);
}

QGraphicsItem * DataStructureBackendManager::pointerItem(PointerPtr pointer) const
{
    if (DataStructureBackendInterface *plg = pointer->dataStructure()->document()->backend()) {
        return plg->pointerItem(pointer);
    }
    return d->_currentPlugin->pointerItem(pointer);;
}

QLayout * DataStructureBackendManager::pointerExtraProperties(PointerPtr pointer, QWidget *parent) const
{
    if (DataStructureBackendInterface *plg = pointer->dataStructure()->document()->backend()) {
        return plg->pointerExtraProperties(pointer, parent);
    }
    return d->_currentPlugin->pointerExtraProperties(pointer, parent);
}

QLayout * DataStructureBackendManager::dataStructureExtraProperties(DataStructurePtr dataStructure, QWidget *parent) const
{
    if (DataStructureBackendInterface *plg = dataStructure->document()->backend()) {
        return plg->dataStructureExtraProperties(dataStructure, parent);
    }
    return d->_currentPlugin->dataStructureExtraProperties(dataStructure, parent);
}

QLayout * DataStructureBackendManager::dataExtraProperties(DataPtr data, QWidget *parent) const
{
    if (DataStructureBackendInterface *plg = data->dataStructure()->document()->backend()) {
        return plg->dataExtraProperties(data, parent);
    }
    return d->_currentPlugin->dataExtraProperties(data, parent);
}
