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

#include "toolspluginmanager.h"
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include "ToolsPlugins.h"


ToolsPluginManager * ToolsPluginManager::self = 0;


ToolsPluginManager * ToolsPluginManager::New()
{
  if (ToolsPluginManager::self == 0){
      ToolsPluginManager::self = new ToolsPluginManager;
  }
  return ToolsPluginManager::self;
}


void ToolsPluginManager::loadPlugins(){  
     ToolsPluginInterface * toolsPlugins;
     
     _names.clear();
     _category.clear();
     _toolTip.clear();
     
     
     //SelectDirs...
     QDir pluginsDir (QCoreApplication::applicationDirPath());
     if (pluginsDir.exists("plugins")){
	pluginsDir.cd("plugins");
	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
	    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
	    QObject *plugin = pluginLoader.instance();
	    if (plugin) {
		toolsPlugins = qobject_cast<ToolsPluginInterface *>(plugin);
		if (toolsPlugins){
		    _names.append (toolsPlugins->name());
		    _toolTip.append (toolsPlugins->tooltip());
		    _category.append (toolsPlugins->category());
		    _plugins.append (plugin);
		}
	    }
	}
     }
 }