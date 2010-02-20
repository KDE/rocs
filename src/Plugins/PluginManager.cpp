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

class PluginManagerPrivate {
  public:
      PluginManagerPrivate(){
	  toolsPluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/ToolPlugin"));
	  filePluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/FilePlugin"));
      }
      ~PluginManagerPrivate(){
	
      }
    KPluginInfo PluginInfoFromName(const QString &arg1){
	foreach (KPluginInfo info, toolsPluginsInfo){
	    if (info.name() == arg1){
		return info;
	    }
	}
	return KPluginInfo();
    }
      typedef KPluginInfo::List KPluginList;
      KPluginList toolsPluginsInfo;
      KPluginList filePluginsInfo;
      
      QMap<KPluginInfo,  ToolsPluginInterface*> toolsPluginsMap;
      QMap<KPluginInfo,  FilePluginInterface*> filePluginsMap;
  
};
  
  
  
  
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
  _d = new PluginManagerPrivate();
}


PluginManager::~PluginManager()
{
  kDebug() << " ++++++++++++++++ NEED DELETE PLUGINS!!!! A MEMORY LEAK! Running away!";
  
  delete _d;
}


void PluginManager::loadPlugins() {
  loadFilePlugins();
  loadToolsPlugins();  
}

bool PluginManager::loadToolPlugin(QString name){
    KPluginInfo kpi =  _d->PluginInfoFromName(name);
    if (kpi.isValid()){
	QString error;
	ToolsPluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<ToolsPluginInterface>( QString::fromLatin1( "Rocs/ToolPlugin" ), QString::fromLatin1( "[Name]=='%1'" ).arg( name ), this, QVariantList(), &error );
	if (plugin ){
	    _d->toolsPluginsMap.insert(kpi, plugin);
	    kpi.setPluginEnabled(true);
	    return true;
	}
	else {
	  kDebug() << "error loading plugin: " << name << error;
	}
	
    }else {
	kDebug() << "Error loading tool plugin " << name;
    }
    return false;
}

void PluginManager::loadToolsPlugins() {

    kDebug() << "Load Tools plugins";
    
    foreach (KPluginInfo info, _d->toolsPluginsInfo){
	loadToolPlugin(info.name());
    }
}


KPluginInfo PluginManager::pluginInfo( Rocs::ToolsPluginInterface* plugin)
{
      return _d->toolsPluginsMap.key(plugin);
}

QList< ToolsPluginInterface* > PluginManager::toolPlugins()
{
      QList < ToolsPluginInterface * > value;
      foreach (KPluginInfo info, _d->toolsPluginsMap.keys()){
	  if (info.isPluginEnabled()){
	      value.append(_d->toolsPluginsMap[info]);
	  }
      }
      
      return value;
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
