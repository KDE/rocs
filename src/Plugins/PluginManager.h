/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H
#include <QList>

#include "ToolsPluginInterface.h"
#include "FilePluginInterface.h"
#include "rocslib_export.h"

class KPluginInfo;

class ROCSLIB_EXPORT PluginManager: public QObject
{
    Q_OBJECT
private:
    //QList<ToolsPluginInterface*> _toolPlugins;
    QList<FilePluginInterface*> _filePlugins;

    PluginManager();

    static PluginManager * self;

    /**The D Pointer*/
    class PluginManagerPrivate * _d;
public:

    static PluginManager * instance();
    ~PluginManager();

    void loadPlugins();


    bool loadToolPlugin(QString arg1);

    void loadToolsPlugins();

    void loadFilePlugins();

    QList < ToolsPluginInterface*> toolPlugins();
    QList < FilePluginInterface*> filePlugins() const;

    KPluginInfo pluginInfo(ToolsPluginInterface * plugin);

    KPluginInfo pluginInfo(FilePluginInterface * plugin);

    /** Returns plugin that can handle extension \p ext
    \param ext File extension (like 'TXT', 'cpp', '.Cpp', '*.js')
    \return File plugin to handle files with that extension or 0 if there is no plugin to handle it.
    */
    FilePluginInterface *  filePluginsByExtension(QString ext);

};

#endif // PLUGINMANAGER_H
