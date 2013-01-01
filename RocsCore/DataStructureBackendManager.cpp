/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include <KDebug>

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
        KService::List services = trader->query(serviceType);
        _pluginInfo = KPluginInfo::fromServices(services);

        // load backends
        for (KPluginList::iterator iter = _pluginInfo.begin(); iter != _pluginInfo.end(); ++iter) {
            if (!iter->isValid()) {
                kError() << "Error loading data structure backend: " << iter->name();
                continue;
            }

            QString error;
            DataStructureBackendInterface *plugin =
                KServiceTypeTrader::createInstanceFromQuery<DataStructureBackendInterface>(
                                        QString::fromLatin1("Rocs/DataStructurePlugin"),
                                        QString::fromLatin1("[Name]=='%1'").arg(iter->name()),
                                        _parent,
                                        QVariantList(),
                                        &error);

            if (plugin) {
                QString identifier = iter->property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString();
                if (identifier.isEmpty()) {
                    identifier = iter->name();
                    kWarning() << "No data structure backend identifier exists for plugin " << identifier << ", using its name.";
                }
                _pluginList.insert(identifier, plugin);
                iter->setPluginEnabled(true);
                continue;
            }
            kError() << "Error while loading data structure backend \"" << iter->name() << "\": " << error;
        }

        if (_pluginList.isEmpty()) {
            kError() << "No backend found, cannot set active data structure backend.";
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
        kError() << "Could not set \"" << pluginIdentifier << "\" as current backend, aborting.";
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
