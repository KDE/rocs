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
#include "DataType.h"

namespace Rocs {
class DSPluginManagerPrivate {
public:
    DSPluginManagerPrivate(QObject * parent):m_parent(parent) {
        m_actualPlugin = 0;
    }

    DataType* changeToDS ( DataType* dataType , DataTypeDocument * parent) {
        if ( m_actualPlugin ) {

            return m_actualPlugin->changeToDS ( dataType, parent );
        }
        return dataType;
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
    DataType* createNewDS ( DataTypeDocument* arg1 , const QString & pluginName ) {
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
    }
    void setActivePlugin(const QString &pluginName) {
        if (Rocs::DSPluginInterface * plg = plugin(pluginName)) {
            m_actualPlugin = plg;
        }
    }


    DSPluginInterface * m_actualPlugin;

private:

    typedef KPluginInfo::List KPluginList;
    KPluginList m_DSPluginsInfo;

    QHash<QString, DSPluginInterface*> m_plugins;
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

Rocs::DSPluginManager::DSPluginManager(QObject *parent) : QObject(parent),_d ( new DSPluginManagerPrivate(this) ) {

    _d->loadPlugins();
}

Rocs::DSPluginManager* Rocs::DSPluginManager::instance(QObject * parent) {
    if ( DSPluginManager::self == 0 ) {
        DSPluginManager::self = new DSPluginManager(parent);
    }
    return DSPluginManager::self;
}


void Rocs::DSPluginManager::changeActiveDS()
{
    QAction *action = qobject_cast<QAction *> ( sender() );

    if (! action ) {
        return;
    }
    changeActiveDS ( Rocs::DSPluginManager::instance()->pluginsList().at(action->data().toInt() )->name() );

}


void Rocs::DSPluginManager::changeActiveDS (const QString &newDS ) {
    if ( listOfDS().contains ( newDS ) && newDS != _d->actualPluginName()) {

//         kDebug() << newDS << _d->actualPluginName();
        _d->setActivePlugin(newDS);
        emit changingDS ( newDS );
        emit DSChangedTo ( newDS );
    }
}

DataType* Rocs::DSPluginManager::changeToDS ( DataType* dataType, DataTypeDocument * parent ) {
    dataType->beforeConvert();
    return instance()->_d->changeToDS ( dataType , parent);

}

DataType* Rocs::DSPluginManager::createNewDS ( DataTypeDocument* parent , const QString & pluginName) {
    return instance()->_d->createNewDS(parent, pluginName);

}

KPluginInfo Rocs::DSPluginManager::pluginInfo( Rocs::DSPluginInterface* plugin) const {
    return _d->pluginInfo(plugin);
}

const QStringList Rocs::DSPluginManager::listOfDS() {
    return instance()->_d->listOfDS();
}

QList< Rocs::DSPluginInterface* > Rocs::DSPluginManager::pluginsList()
{
    return instance()->_d->pluginList();
}

QString Rocs::DSPluginManager::actualPlugin() const
{
    return _d->actualPluginName();
}

QGraphicsItem* Rocs::DSPluginManager::datumItem(Datum* datum)
{
  return _d->m_actualPlugin->datumItem(datum);
}

QGraphicsItem* Rocs::DSPluginManager::pointerItem ( Pointer* pointer ){
    return _d->m_actualPlugin->pointerItem(pointer);;
}

QLayout* Rocs::DSPluginManager::pointerExtraProperties ( Pointer* pointer, QWidget* parent ){
  return _d->m_actualPlugin->pointerExtraProperties(pointer, parent);
}

QLayout* Rocs::DSPluginManager::dataTypeExtraProperties ( DataType* dataType, QWidget* parent ){
      return _d->m_actualPlugin->dataTypeExtraProperties(dataType, parent);
}

QLayout* Rocs::DSPluginManager::datumExtraProperties ( Datum* datum, QWidget* parent ){
    return _d->m_actualPlugin->datumExtraProperties(datum, parent);
}


