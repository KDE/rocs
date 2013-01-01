/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include "ToolsPluginInterface.h"
#include <QList>

class KPluginInfo;
class ToolManagerPrivate;

/**
 * \class ToolManager
 *
 * This singelton class provides access to all available visual graph editor tools.
 */
class VISUALEDITOR_EXPORT ToolManager
    : public QObject
{
    Q_OBJECT

public:
    /**
     * Returns self reference to tool manager. First call of this method loads all available tools.
     *
     * \return self reference
     */
    static ToolManager & self();

    /**
     * Return list of available tool plugins.
     */
    QList<ToolsPluginInterface*> plugins() const;

    /**
     * \return plugin info for specified tool plugin.
     */
    KPluginInfo pluginInfo(ToolsPluginInterface* plugin);

private:
    /**
     * \internal
     * Private constructor.
     */
    ToolManager();
    ~ToolManager();

    // make non-copyable
    ToolManager(const ToolManager&);
    void operator=(const ToolManager&);

    // d pointer
    const QScopedPointer<ToolManagerPrivate> d;
};

#endif
