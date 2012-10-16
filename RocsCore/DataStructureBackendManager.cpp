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
#include "DataStructurePluginInterface.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KDebug>

#include <QHash>
#include <QAction>
#include "DataStructure.h"
#include <KMessageBox>
#include "Data.h"
#include "Pointer.h"
#include <boost/scoped_ptr.hpp>

DataStructureBackendManager* DataStructureBackendManager::instance = 0;


class DataStructureBackendManagerPrivate
{
public:

    typedef KPluginInfo::List KPluginList;

    QObject* _parent;
    KPluginList _pluginInfo;
    DataStructurePluginInterface* _currentPlugin;
    QHash<QString, DataStructurePluginInterface*> _pluginList;

    DataStructureBackendManagerPrivate(QObject * parent)
        : _parent(parent),
        _currentPlugin(0)
    {
    }

    ~DataStructureBackendManagerPrivate()
    {
        _currentPlugin = 0;

        // clear plugins
        qDeleteAll<QHash<QString, DataStructurePluginInterface*>::iterator>(_pluginList.begin(), _pluginList.end());
        _pluginList.clear();
    }

    DataStructurePtr convertDataStructureToActiveBackend(DataStructurePtr dataStructure, Document * parent)
    {
        if (_currentPlugin) {
            return _currentPlugin->convertToDataStructure(dataStructure, parent);
        }
        return dataStructure;
    }

    KPluginInfo pluginInfo(DataStructurePluginInterface* plugin)
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

    DataStructurePtr createDataStructure(Document* arg1 , const QString & pluginName)
    {
        if (!pluginName.isEmpty()) {
            if (DataStructurePluginInterface * plugin = _pluginList.value(pluginName)) {
                return plugin->createDataStructure(arg1);
            }
        } else if (_currentPlugin) {
            return _currentPlugin->createDataStructure(arg1);
        }
        return DataStructurePtr();
    }

    DataStructurePluginInterface* backend(const QString& pluginIdentifier)
    {
        if (_pluginList.contains(pluginIdentifier)) {
            return _pluginList[pluginIdentifier];
        }
        return 0;
    }

    void setActivePlugin(const QString& pluginIdentifier)
    {
        if (DataStructurePluginInterface * plg = backend(pluginIdentifier)) {
            _currentPlugin = plg;
        }
    }

    bool loadBackend(KPluginInfo& pluginInfo)
    {
        if (!pluginInfo.isValid()) {
            kError() << "Error loading data structure backend: " << pluginInfo.name();
            return false;
        }

        QString error;
        DataStructurePluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<DataStructurePluginInterface> (QString::fromLatin1("Rocs/DataStructurePlugin"), QString::fromLatin1("[Name]=='%1'").arg(pluginInfo.name()), _parent, QVariantList(), &error);

        if (plugin) {
            QString identifier = pluginInfo.property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString();
            if (identifier.isEmpty()) {
                identifier = pluginInfo.name();
                kWarning() << "No data structure backend identifier exists for plugin " << identifier << ", using its name.";
            }
            _pluginList.insert(identifier, plugin);
            pluginInfo.setPluginEnabled(true);
            return true;
        }
        kError() << "Error while loading data structure backend \"" << pluginInfo.name() << "\": " << error;
        return false;
    }

    void loadBackends()
    {
        _pluginInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/DataStructurePlugin"));

        foreach(KPluginInfo info, _pluginInfo) {
            loadBackend(info);
        }

        if (_pluginList.isEmpty()) {
            kError() << "No backend found, cannot set active data structure backend.";
            return;
        }
        foreach (DataStructurePluginInterface * i, _pluginList.values()) {
            if (i->internalName() == QLatin1String("Graph")){
                _currentPlugin = i;
                return;
            }
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

void DataStructureBackendManager::loadBackends()
{
    d->loadBackends();
}

DataStructureBackendManager* DataStructureBackendManager::self()
{
    if (DataStructureBackendManager::instance == 0) {
        instance = new DataStructureBackendManager();
        instance->loadBackends();
    }
    return instance;
}

void DataStructureBackendManager::setBackend(const QString& pluginIdentifier)
{
    DataStructurePluginInterface* plugin = d->backend(pluginIdentifier);
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

DataStructurePtr DataStructureBackendManager::convertDataStructureToActiveBackend(DataStructurePtr dataStructure, Document * parent)
{
    return d->convertDataStructureToActiveBackend(dataStructure, parent);
}

DataStructurePtr DataStructureBackendManager::createDataStructure(Document* parent , const QString& pluginName)
{
    return d->createDataStructure(parent, pluginName);
}

KPluginInfo DataStructureBackendManager::backendInfo(DataStructurePluginInterface* plugin) const
{
    return d->pluginInfo(plugin);
}

const QStringList DataStructureBackendManager::backends() const
{
    return d->_pluginList.keys();
}

DataStructurePluginInterface* DataStructureBackendManager::activeBackend() const
{
    return d->_currentPlugin;
}

DataStructurePluginInterface* DataStructureBackendManager::backend(const QString& internalName) const
{
    return d->backend(internalName);
}

QGraphicsItem* DataStructureBackendManager::dataItem(DataPtr data) const
{
    if (DataStructurePluginInterface * plg = data->dataStructure()->document()->dataStructurePlugin()) {
        return plg->dataItem(data);
    }
    return d->_currentPlugin->dataItem(data);
}

QGraphicsItem* DataStructureBackendManager::pointerItem(PointerPtr pointer) const
{
    if (DataStructurePluginInterface * plg = pointer->dataStructure()->document()->dataStructurePlugin()) {
        return plg->pointerItem(pointer);
    }
    return d->_currentPlugin->pointerItem(pointer);;
}

QLayout* DataStructureBackendManager::pointerExtraProperties(PointerPtr pointer, QWidget* parent) const
{
    if (DataStructurePluginInterface * plg = pointer->dataStructure()->document()->dataStructurePlugin()) {
        return plg->pointerExtraProperties(pointer, parent);
    }
    return d->_currentPlugin->pointerExtraProperties(pointer, parent);
}

QLayout* DataStructureBackendManager::dataStructureExtraProperties(DataStructurePtr dataStructure, QWidget* parent) const
{
    if (DataStructurePluginInterface * plg = dataStructure->document()->dataStructurePlugin()) {
        return plg->dataStructureExtraProperties(dataStructure, parent);
    }
    return d->_currentPlugin->dataStructureExtraProperties(dataStructure, parent);
}

QLayout* DataStructureBackendManager::dataExtraProperties(DataPtr data, QWidget* parent) const
{
    if (DataStructurePluginInterface * plg = data->dataStructure()->document()->dataStructurePlugin()) {
        return plg->dataExtraProperties(data, parent);
    }
    return d->_currentPlugin->dataExtraProperties(data, parent);
}
