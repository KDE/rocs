/*

    Copyright (C) 2010  Wagner Reck (wagner.reck@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

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

class DataStructurePluginManagerPrivate {
public:
DataStructurePluginManagerPrivate(QObject * parent):m_parent(parent) {
    m_actualPlugin = 0;
}

DataStructure* changeToDataStructure ( DataStructure* dataStructure, Document * parent) {
    if ( m_actualPlugin ) {
        return m_actualPlugin->changeToDataStructure ( dataStructure,  parent);
    }
    return dataStructure;
}
void loadPlugins() {
    loadDataStructurePlugins();
    if (m_plugins.isEmpty()){
        return;
    }
    m_actualPlugin = pluginList().last();
}

KPluginInfo pluginInfo ( DataStructurePluginInterface* plugin ) {
    QString name = m_plugins.key(plugin);
    foreach (const KPluginInfo& inf, m_DataStructurePluginsInfo) {
        if (inf.name() == name) {
            return inf;
        }
    }
    return KPluginInfo();
}

DataStructure* createNewDataStructure ( Document* arg1 , const QString & pluginName ) {
    if (!pluginName.isEmpty()) {
        if (DataStructurePluginInterface * plugin = m_plugins.value(pluginName)) {
            return plugin->createDataStructure ( arg1 );
        }
    } else if ( m_actualPlugin ) {
        return m_actualPlugin->createDataStructure ( arg1 );
    }
    return 0;
}

DataStructurePluginInterface* plugin(const QString &pluginName) {
    DataStructurePluginInterface * plugin = m_plugins.value(pluginName);
    if (plugin){
        return plugin;
    }
    return 0;
}

void setActivePlugin(const QString &pluginName) {
    if (DataStructurePluginInterface * plg = plugin(pluginName)) {
        m_actualPlugin = plg;
    }
}

QStringList listOfDataStructures() const                  { return m_plugins.keys(); }
QList <DataStructurePluginInterface*> pluginList() const { return m_plugins.values(); }
QString actualPluginName() const                         { return m_actualPlugin->name(); }



typedef KPluginInfo::List KPluginList;

KPluginList m_DataStructurePluginsInfo;
DataStructurePluginInterface * m_actualPlugin;
QHash<QString, DataStructurePluginInterface*> m_plugins;
QObject* m_parent;


bool loadDataStructurePlugin ( KPluginInfo & pluginInfo ) {
//         KPluginInfo kpi =  pluginInfoFromName ( name );
    if ( pluginInfo.isValid() ) {
        QString error;
        DataStructurePluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<DataStructurePluginInterface> ( QString::fromLatin1 ( "Rocs/DataStructurePlugin" ), QString::fromLatin1 ( "[Name]=='%1'" ).arg ( pluginInfo.name() ), m_parent, QVariantList(), &error );
        if ( plugin ) {
            m_plugins.insert ( pluginInfo.name(), plugin );
            pluginInfo.setPluginEnabled ( true );
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

    foreach ( KPluginInfo info, m_DataStructurePluginsInfo ) {
        loadDataStructurePlugin ( info );
    }
}
};

DataStructurePluginManager* DataStructurePluginManager::_self = 0;

DataStructurePluginManager::DataStructurePluginManager() :_d ( new DataStructurePluginManagerPrivate(this) ) {
    _d->loadPlugins();
}

DataStructurePluginManager::~DataStructurePluginManager()
{
    delete _d;
}

DataStructurePluginManager* DataStructurePluginManager::self() {
    if ( !_self ){
      _self = new DataStructurePluginManager();
    }
    return _self;
}

void DataStructurePluginManager::changeActiveDataStructure()
{
    QAction *action = qobject_cast<QAction *> ( sender() );
    if (! action ) {
        return;
    }
    //Check if is a valid index
    if (action->data().toInt() < pluginsList().count()){
        changeActiveDataStructure ( DataStructurePluginManager::self()->pluginsList().at(action->data().toInt() )->name() );
    }
}

void DataStructurePluginManager::changeActiveDataStructure (const QString &newDataStructure ) {
    if ( listOfDataStructures().contains ( newDataStructure ) && newDataStructure  != _d->actualPluginName()) {
        _d->setActivePlugin(newDataStructure);
        emit changingDataStructure ( newDataStructure );
    }
}

DataStructure* DataStructurePluginManager::changeToDataStructure ( DataStructure* dataStructure, Document * parent ) {
    return _d->changeToDataStructure ( dataStructure, parent );

}

DataStructure* DataStructurePluginManager::createNewDataStructure ( Document* parent , const QString & pluginName) {
    return _d->createNewDataStructure(parent, pluginName);

}

KPluginInfo DataStructurePluginManager::pluginInfo( DataStructurePluginInterface* plugin) const {
    return _d->pluginInfo(plugin);
}

const QStringList DataStructurePluginManager::listOfDataStructures() {
    return _d->listOfDataStructures();
}

QList< DataStructurePluginInterface* > DataStructurePluginManager::pluginsList()
{
    return _d->pluginList();
}

QString DataStructurePluginManager::actualPlugin() const
{
    return _d->actualPluginName();
}

QGraphicsItem* DataStructurePluginManager::dataItem(Data* data)
{
    return _d->m_actualPlugin->dataItem(data);
}

QGraphicsItem* DataStructurePluginManager::pointerItem ( Pointer* pointer ){
    return _d->m_actualPlugin->pointerItem(pointer);;
}

QLayout* DataStructurePluginManager::pointerExtraProperties ( Pointer* pointer, QWidget* parent ){
    return _d->m_actualPlugin->pointerExtraProperties(pointer, parent);
}

QLayout* DataStructurePluginManager::dataStructureExtraProperties ( DataStructure* dataStructure, QWidget* parent ){
    return _d->m_actualPlugin->dataStructureExtraProperties(dataStructure, parent);
}

QLayout* DataStructurePluginManager::dataExtraProperties ( Data* data, QWidget* parent ){
    return _d->m_actualPlugin->dataExtraProperties(data, parent);
}


