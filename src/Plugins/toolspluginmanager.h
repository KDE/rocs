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
#include "ToolsPlugins.h"

class ToolsPluginManager
{
  QStringList _names;
  QStringList _category;
  QStringList _toolTip;
  QList<QObject*> _plugins;
  ToolsPluginManager() { } 
  
  static ToolsPluginManager * self;
  public:
    
    static ToolsPluginManager * New();
    ~ToolsPluginManager() { }
    void loadPlugins();
    
    QStringList pluginNames() { return _names;}
    QStringList pluginToolTips() { return _toolTip;}
    QStringList pluginCategories() { return _category;}
    QList <QObject*> plugins(){return _plugins;}
  
};

#endif // TOOLSPLUGINMANAGER_H
