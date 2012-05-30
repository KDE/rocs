/*
    This file is part of Rocs.
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

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

#include "DataStructurePluginManager.h"
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
#include <boost/shared_ptr.hpp>

DataStructurePluginManager* DataStructurePluginManager::_self = 0;

// ----------------------------------- D - Pointer ---------------------------------

class DataStructurePluginManagerPrivate
{
public:

    typedef KPluginInfo::List KPluginList;

    KPluginList m_DataStructurePluginsInfo;
    DataStructurePluginInterface * m_actualPlugin;
    QHash<QString, DataStructurePluginInterface*> m_plugins;
    QObject* m_parent;

    DataStructurePluginManagerPrivate(QObject * parent): m_parent(parent) {
        m_actualPlugin = 0;
    }

    DataStructurePtr changeToDataStructure(DataStructurePtr dataStructure, Document * parent) {
        if (m_actualPlugin) {
            return m_actualPlugin->convertToDataStructure(dataStructure,  parent);
        }
        return dataStructure;
    }
    void loadPlugins() {
        loadDataStructurePlugins();
        if (m_plugins.isEmpty()) {
            return;
        }
        m_actualPlugin = pluginList().last();
    }

    KPluginInfo pluginInfo(DataStructurePluginInterface* plugin) {
        QString name = m_plugins.key(plugin);
        foreach(const KPluginInfo & inf, m_DataStructurePluginsInfo) {
            if (inf.name() == name) {
                return inf;
            }
        }
        return KPluginInfo();
    }

    DataStructurePtr createNewDataStructure(Document* arg1 , const QString & pluginName) {
        if (!pluginName.isEmpty()) {
            if (DataStructurePluginInterface * plugin = m_plugins.value(pluginName)) {
                return plugin->createDataStructure(arg1);
            }
        } else if (m_actualPlugin) {
            return m_actualPlugin->createDataStructure(arg1);
        }
        return DataStructurePtr();
    }

    DataStructurePluginInterface* pluginByInternalName(const QString &pluginIdentifier){
        foreach(DataStructurePluginInterface* plg, pluginList()) {
            if (plg->internalName() == pluginIdentifier) {
                return plg;
            }
        }
        return 0;
    }

    void setActivePlugin(const QString &pluginIdentifier) {
        if (DataStructurePluginInterface * plg = pluginByInternalName(pluginIdentifier)) {
            m_actualPlugin = plg;
        }
    }

    bool loadDataStructurePlugin(KPluginInfo & pluginInfo) {
//         KPluginInfo kpi =  pluginInfoFromName ( name );
        if (pluginInfo.isValid()) {
            QString error;
            DataStructurePluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<DataStructurePluginInterface> (QString::fromLatin1("Rocs/DataStructurePlugin"), QString::fromLatin1("[Name]=='%1'").arg(pluginInfo.name()), m_parent, QVariantList(), &error);
            if (plugin) {
                m_plugins.insert(pluginInfo.name(), plugin);
                pluginInfo.setPluginEnabled(true);
                return true;
            } else {
                kDebug() << "error loading plugin: " << pluginInfo.name() << error;
            }

        } else {
            kDebug() << "Error loading tool plugin " << pluginInfo.name();
        }
        return false;
    }

    void loadDataStructurePlugins() {
        m_DataStructurePluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/DataStructurePlugin"));
        kDebug() << "Load Tools plugins";

        foreach(KPluginInfo info, m_DataStructurePluginsInfo) {
            loadDataStructurePlugin(info);
        }
    }

    QStringList listOfDataStructures() const                 {
        return m_plugins.keys();
    }
    QList <DataStructurePluginInterface*> pluginList() const {
        return m_plugins.values();
    }
    QString actualPluginName() const                         {
        return m_actualPlugin->name();
    }

};

// ----------------------------------- Class  ---------------------------------

DataStructurePluginManager::DataStructurePluginManager() : _d(new DataStructurePluginManagerPrivate(this))
{
    _d->loadPlugins();
}

DataStructurePluginManager::~DataStructurePluginManager()
{
    delete _d;
}

DataStructurePluginManager* DataStructurePluginManager::self()
{
    if (!_self) {
        _self = new DataStructurePluginManager();
    }
    return _self;
}

void DataStructurePluginManager::setDataStructurePlugin()
{
    //TODO this is highly fragile code
    QAction *action = qobject_cast<QAction *> (sender());
    if (! action) {
        return;
    }

    int actionIndex = action->data().toInt();
    if (actionIndex >= pluginsList().count()) {
        return;
    }
    kDebug() << "Setting the data structure plugin to" << pluginsList().at(actionIndex)->internalName() ;
    setDataStructurePlugin(pluginsList().at(actionIndex)->internalName());
}

void DataStructurePluginManager::setDataStructurePlugin(const QString &pluginIdentifier)
{
    DataStructurePluginInterface* plugin = _d->pluginByInternalName(pluginIdentifier);
    if (!plugin) {
        return;
    }
    if (_d->m_actualPlugin && pluginIdentifier == _d->m_actualPlugin->internalName()) {
        return;
    }

    kDebug() << "Setting as active plugin: "<< pluginIdentifier;

    _d->setActivePlugin(pluginIdentifier);
    emit changingDataStructurePlugin(pluginIdentifier);
}

DataStructurePtr DataStructurePluginManager::changeToDataStructure(DataStructurePtr dataStructure, Document * parent)
{
    return _d->changeToDataStructure(dataStructure, parent);
}

DataStructurePtr DataStructurePluginManager::createNewDataStructure(Document* parent , const QString & pluginName)
{
    return _d->createNewDataStructure(parent, pluginName);

}

KPluginInfo DataStructurePluginManager::pluginInfo(DataStructurePluginInterface* plugin) const
{
    return _d->pluginInfo(plugin);
}

const QStringList DataStructurePluginManager::listOfDataStructures()
{
    return _d->listOfDataStructures();
}

QList< DataStructurePluginInterface* > DataStructurePluginManager::pluginsList()
{
    return _d->pluginList();
}

QString DataStructurePluginManager::pluginName() const
{
    return _d->actualPluginName();
}
DataStructurePluginInterface* DataStructurePluginManager::actualPlugin()
{
    return _d->m_actualPlugin;
}

DataStructurePluginInterface* DataStructurePluginManager::plugin(const QString& internalName) const
{
    return _d->pluginByInternalName(internalName);
}


QGraphicsItem* DataStructurePluginManager::dataItem(DataPtr data)
{
    if (DataStructurePluginInterface * plg = data->dataStructure()->document()->dataStructurePlugin())
        return plg->dataItem(data);
    return _d->m_actualPlugin->dataItem(data);
}

QGraphicsItem* DataStructurePluginManager::pointerItem(PointerPtr pointer)
{
    if (DataStructurePluginInterface * plg = pointer->dataStructure()->document()->dataStructurePlugin())
        return plg->pointerItem(pointer);
    return _d->m_actualPlugin->pointerItem(pointer);;
}

QLayout* DataStructurePluginManager::pointerExtraProperties(PointerPtr pointer, QWidget* parent)
{
    if (DataStructurePluginInterface * plg = pointer->dataStructure()->document()->dataStructurePlugin())
        return plg->pointerExtraProperties(pointer, parent);
    return _d->m_actualPlugin->pointerExtraProperties(pointer, parent);
}

QLayout* DataStructurePluginManager::dataStructureExtraProperties(DataStructurePtr dataStructure, QWidget* parent)
{
    if (DataStructurePluginInterface * plg = dataStructure->document()->dataStructurePlugin())
        return plg->dataStructureExtraProperties(dataStructure, parent);
    return _d->m_actualPlugin->dataStructureExtraProperties(dataStructure, parent);
}

QLayout* DataStructurePluginManager::dataExtraProperties(DataPtr data, QWidget* parent)
{
    if (DataStructurePluginInterface * plg = data->dataStructure()->document()->dataStructurePlugin())
        return plg->dataExtraProperties(data, parent);
    return _d->m_actualPlugin->dataExtraProperties(data, parent);
}
