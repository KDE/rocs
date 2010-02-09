/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "PluginManager.h"
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include "ToolsPluginInterface.h"
#include <KServiceTypeTrader>
#include <KPluginInfo>
#include <KDebug>
#include <KMessageBox>


namespace Rocs {
PluginManager * PluginManager::self = 0;


PluginManager * PluginManager::New()
{
    if (PluginManager::self == 0) {
        PluginManager::self = new PluginManager;
    }
    return PluginManager::self;
}


PluginManager::PluginManager()
{

}

void PluginManager::loadPlugins() {
  loadFilePlugins();
  loadToolsPlugins();  
}

void PluginManager::loadToolsPlugins() {

    kDebug() << "Load all plugins";
    KService::List offers = KServiceTypeTrader::self()->query("Rocs/ToolPlugin");

    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter)
    {
        QString error;
        KService::Ptr service = *iter;

        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory)
        {
            KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
                                       service->library()));
            kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

        ToolsPluginInterface *plugin = factory->create<ToolsPluginInterface>(this);

        if (plugin) {
            kDebug() << "Load plugin:" << service->name();
            kError(5001) << "<html><p>KPluginFactory load the plugin:<br/><i>%1</i></p></html>";
            //emit pluginLoaded(plugin);
        } else {
            //KMessageBox::error(0, i18n("<html><p>Plugin Error:<br/><i>%1</i></p></html>",
            //                           service->name()));
            kDebug() << error;
        }
    }
//      ToolsPluginInterface * toolsPlugins;
//
//      //SelectDirs...
//      QDir pluginsDir (QCoreApplication::applicationDirPath());
//      if (pluginsDir.exists("plugins")){
// 	pluginsDir.cd("plugins");
// 	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
// 	    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
// 	    QObject *plugin = pluginLoader.instance();
// 	    if (plugin) {
// 		toolsPlugins = qobject_cast<ToolsPluginInterface *>(plugin);
// 		if (toolsPlugins){
//
// 		    _plugins.append (plugin);
// 		}
// 	    }
// 	}
//      }
}


void PluginManager::loadFilePlugins() {

    kDebug() << "Load all plugins";
    KService::List offers = KServiceTypeTrader::self()->query("Rocs/FilePlugin");

    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter)
    {
        QString error;
        KService::Ptr service = *iter;

        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory)
        {
            KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
                                       service->library()));
            kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

        ToolsPluginInterface *plugin = factory->create<ToolsPluginInterface>(this);

        if (plugin) {
            kDebug() << "Load plugin:" << service->name();
            kError(5001) << "<html><p>KPluginFactory load the plugin:<br/><i>%1</i></p></html>";
            //emit pluginLoaded(plugin);
        } else {
            //KMessageBox::error(0, i18n("<html><p>Plugin Error:<br/><i>%1</i></p></html>",
            //                           service->name()));
            kDebug() << error;
        }
    }
}
KPluginInfo pluginInfo(const ToolsPluginInterface * /*plugin*/) {
    return KPluginInfo();
}
}
