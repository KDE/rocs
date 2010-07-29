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

#include "DSPluginManager.h"
#include "DSPluginInterface.h"

#include <KPluginInfo>
#include <KServiceTypeTrader>
#include <KDebug>

#include <QHash>
namespace Rocs {
class DSPluginManagerPrivate {
public:
    DSPluginManagerPrivate(QObject * parent):m_parent(parent){
      m_actualPlugin = 0;
    }

    Graph* changeToDS ( Graph* graph ) {
        if ( m_actualPlugin ) {
            return m_actualPlugin->changeToDS ( graph );
        }
        return graph;
    }
    void loadPlugins() {
        loadDSPlugins();
        if (plugins.isEmpty())
          return;
        m_actualPlugin = pluginList().at(0);
    }

    KPluginInfo pluginInfo ( DSPluginInterface* plugin ) {
        QString name = plugins.key(plugin);
        foreach (KPluginInfo inf, m_DSPluginsInfo){
          if (inf.name() == name){
              return inf;
          }
        }
        return KPluginInfo();
    }
    Graph* createNewDS ( GraphDocument* arg1 ){
      if ( m_actualPlugin ) {
          return m_actualPlugin->createDS ( arg1 );
      }
      return 0;
    }
    QStringList listOfDS(){
      return plugins.keys();
    }
    QList < DSPluginInterface*> pluginList(){
      return plugins.values();
    }

private:

    typedef KPluginInfo::List KPluginList;
    KPluginList m_DSPluginsInfo;

    QHash<QString, DSPluginInterface*> plugins;
    DSPluginInterface * m_actualPlugin;
    QObject* m_parent;


    bool loadDSPlugin ( KPluginInfo & pluginInfo ) {
//         KPluginInfo kpi =  pluginInfoFromName ( name );
        if ( pluginInfo.isValid() ) {
            QString error;
            DSPluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<DSPluginInterface> ( QString::fromLatin1 ( "Rocs/DSPlugin" ), QString::fromLatin1 ( "[Name]=='%1'" ).arg ( pluginInfo.name() ), m_parent, QVariantList(), &error );
            if ( plugin ) {
                plugins.insert ( pluginInfo.name(), plugin );
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

    void loadDSPlugins() {
        m_DSPluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/DSPlugin"));
        kDebug() << "Load Tools plugins";

        foreach ( KPluginInfo info, m_DSPluginsInfo ) {
            loadDSPlugin ( info );
        }
    }


};
}

Rocs::DSPluginManager* Rocs::DSPluginManager::self = 0;

Rocs::DSPluginManager::DSPluginManager() :_d ( new DSPluginManagerPrivate(this) ) {

  _d->loadPlugins();
}

Rocs::DSPluginManager* Rocs::DSPluginManager::New() {
    if ( DSPluginManager::self == 0 ) {
        DSPluginManager::self = new DSPluginManager();
    }
    return DSPluginManager::self;
}


void Rocs::DSPluginManager::changeDS ( QString &newDS ) {
    if ( listOfDS().contains ( newDS ) ) {
        emit changingDS ( newDS );
        emit DSChangedTo ( newDS );
        return;
    }
}

Graph* Rocs::DSPluginManager::changeToDS ( Graph* graph ) {

    return New()->_d->changeToDS ( graph );

}

Graph* Rocs::DSPluginManager::createNewDS ( GraphDocument* parent ) {
    return New()->_d->createNewDS(parent);

}

KPluginInfo Rocs::DSPluginManager::pluginInfo( Rocs::DSPluginInterface* plugin) const {
    return _d->pluginInfo(plugin);
}

const QStringList Rocs::DSPluginManager::listOfDS() {
    return New()->_d->listOfDS();
}

QList< Rocs::DSPluginInterface* > Rocs::DSPluginManager::pluginsList()
{
  return New()->_d->pluginList();
}

