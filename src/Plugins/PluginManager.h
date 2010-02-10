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

#ifndef TOOLSPLUGINMANAGER_H
#define TOOLSPLUGINMANAGER_H
#include <QStringList>
#include "ToolsPluginInterface.h"
#include "rocslib_export.h"

class KPluginInfo;

namespace Rocs{
  
class ROCSLIB_EXPORT PluginManager: public QObject
{
  Q_OBJECT
  private:
      QList<ToolsPluginInterface*> _plugins;
      PluginManager();

      static PluginManager * self;
  public:
    
    static PluginManager * New();
    ~PluginManager() { }
    void loadPlugins();
    void loadToolsPlugins();
    void loadFilePlugins();
    
    QList <ToolsPluginInterface*> plugins(){return _plugins;}
    KPluginInfo pluginInfo(const ToolsPluginInterface * plugin);
  
};

}
#endif // TOOLSPLUGINMANAGER_H
