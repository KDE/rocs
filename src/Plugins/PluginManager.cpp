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

#include "ToolsPluginInterface.h"
#include "FilePluginInterface.h"

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

    kDebug() << "Load Tools plugins";
    
    foreach(ToolsPluginInterface * t, _toolPlugins){
	delete t;
    }
    
    _toolPlugins.clear();
    
    KService::List offers = KServiceTypeTrader::self()->query("Rocs/ToolPlugin");

    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter)
    {
        QString error;
        KService::Ptr service = *iter;

        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory)
        {
           kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

        ToolsPluginInterface *plugin = factory->create<ToolsPluginInterface>(this);

        if (plugin) {
            kDebug() << "Loaded plugin:" << service->name();
           _toolPlugins.append(plugin);
            //emit pluginLoaded(plugin);
        } else {
            //KMessageBox::error(0, i18n("<html><p>Plugin Error:<br/><i>%1</i></p></html>",
            //                           service->name()));
            kDebug() << "can´t load Plugin: " << service->name();
        }
    }
}


void PluginManager::loadFilePlugins() {

    kDebug() << "Load File plugins";
    
    foreach (FilePluginInterface * f, _filePlugins){
	delete f;
    }
    _filePlugins.clear();
    
    KService::List offers = KServiceTypeTrader::self()->query("Rocs/FilePlugin");

    KService::List::const_iterator iter;
    for (iter = offers.constBegin(); iter < offers.constEnd(); ++iter)
    {
        QString error;
        KService::Ptr service = *iter;

        KPluginFactory *factory = KPluginLoader(service->library()).factory();

        if (!factory)
        {
            kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
            continue;
        }

        FilePluginInterface *plugin = factory->create<FilePluginInterface>(this);

        if (plugin) {
            kDebug() << "Loaded plugin: " << service->name();
	    _filePlugins.append(plugin);
            
            //emit pluginLoaded(plugin);
        } else {
            kDebug() << "Can't load plugin: " << service->name();
        }
    }
}
KPluginInfo pluginInfo(const ToolsPluginInterface * /*plugin*/) {
    return KPluginInfo();
}

FilePluginInterface *  PluginManager::filePluginsByExtension(QString ext){
    foreach (FilePluginInterface * p,  _filePlugins){
	if (p->extensions().join(";").contains(ext, Qt::CaseInsensitive)){
	    return p;
	}
    }
    return 0;
}

}
