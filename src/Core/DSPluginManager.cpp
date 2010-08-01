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
#include <QAction>

namespace Rocs {
class DSPluginManagerPrivate {
public:
    DSPluginManagerPrivate(QObject * parent):m_parent(parent) {
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
        if (m_plugins.isEmpty())
            return;
        m_actualPlugin = pluginList().last();
    }

    KPluginInfo pluginInfo ( DSPluginInterface* plugin ) {
        QString name = m_plugins.key(plugin);
        foreach (KPluginInfo inf, m_DSPluginsInfo) {
            if (inf.name() == name) {
                return inf;
            }
        }
        return KPluginInfo();
    }
    Graph* createNewDS ( GraphDocument* arg1 , const QString & pluginName ) {
        if (!pluginName.isEmpty()) {
            Rocs::DSPluginInterface * plugin = m_plugins.value(pluginName);
            if (plugin) {
                return plugin->createDS ( arg1 );
            }
        } else {
            if ( m_actualPlugin ) {
                return m_actualPlugin->createDS ( arg1 );
            }
        }
        return 0;
    }
    QStringList listOfDS() {
        return m_plugins.keys();
    }
    QList < DSPluginInterface*> pluginList() {
        return m_plugins.values();
    }
    QString actualPluginName() {
        return m_actualPlugin->name();
    }
    Rocs::DSPluginInterface* plugin(const QString &pluginName) {

        Rocs::DSPluginInterface * plugin = m_plugins.value(pluginName);
        if (plugin)
            return plugin;
        return 0;
//         foreach (Rocs::DSPluginInterface * plugin, plugins.values()) {
//             kDebug() << plugin->name();
//             if (plugin->name() == pluginName) {
//                 return plugin;
//             }
//             return 0;
//         }
    }
    void setActivePlugin(const QString &pluginName) {
        if (Rocs::DSPluginInterface * plg = plugin(pluginName)) {
            m_actualPlugin = plg;
        }
    }

private:

    typedef KPluginInfo::List KPluginList;
    KPluginList m_DSPluginsInfo;

    QHash<QString, DSPluginInterface*> m_plugins;
    DSPluginInterface * m_actualPlugin;
    QObject* m_parent;


    bool loadDSPlugin ( KPluginInfo & pluginInfo ) {
//         KPluginInfo kpi =  pluginInfoFromName ( name );
        if ( pluginInfo.isValid() ) {
            QString error;
            DSPluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<DSPluginInterface> ( QString::fromLatin1 ( "Rocs/DSPlugin" ), QString::fromLatin1 ( "[Name]=='%1'" ).arg ( pluginInfo.name() ), m_parent, QVariantList(), &error );
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


void Rocs::DSPluginManager::changeActiveDS()
{
    QAction *action = qobject_cast<QAction *> ( sender() );

    if (! action ) {
        return;
    }
    changeActiveDS ( Rocs::DSPluginManager::New()->pluginsList().at(action->data().toInt() )->name() );

}


void Rocs::DSPluginManager::changeActiveDS (const QString &newDS ) {
    if ( listOfDS().contains ( newDS ) ) {

        kDebug() << newDS << _d->actualPluginName();
        _d->setActivePlugin(newDS);
        emit changingDS ( newDS );
//         emit DSChangedTo ( newDS );
    }
}

Graph* Rocs::DSPluginManager::changeToDS ( Graph* graph ) {

    return New()->_d->changeToDS ( graph );

}

Graph* Rocs::DSPluginManager::createNewDS ( GraphDocument* parent , const QString & pluginName) {
    return New()->_d->createNewDS(parent, pluginName);

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

QString Rocs::DSPluginManager::actualPlugin() const
{
    return _d->actualPluginName();
}
