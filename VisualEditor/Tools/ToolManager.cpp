/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "ToolManager.h"

#include "ToolsPluginInterface.h"

#include <KServiceTypeTrader>
#include <KPluginInfo>
#include <KDebug>
#include <KMessageBox>

class ToolManagerPrivate
{
public:
    ToolManagerPrivate() {
        toolsPluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/ToolPlugin"));
    }

    ~ToolManagerPrivate() {

    }
    KPluginInfo PluginInfoFromName(const QString &arg1) {
        foreach(KPluginInfo info, toolsPluginsInfo) {
            if (info.name() == arg1) {
                return info;
            }
        }
        return KPluginInfo();
    }
    typedef KPluginInfo::List KPluginList;
    KPluginList toolsPluginsInfo;

    QMap<KPluginInfo,  ToolsPluginInterface*> toolsPluginsMap;
};


ToolManager * ToolManager::self = 0;


ToolManager * ToolManager::instance()
{
    if (ToolManager::self == 0) {
        ToolManager::self = new ToolManager;
    }
    return ToolManager::self;
}


ToolManager::ToolManager()
{
    _d = new ToolManagerPrivate();
}


ToolManager::~ToolManager()
{
    delete _d;
}


void ToolManager::loadPlugins()
{
    loadToolsPlugins();
}


bool ToolManager::loadToolPlugin(QString name)
{
    KPluginInfo kpi =  _d->PluginInfoFromName(name);

    if (kpi.isValid()) {
        QString error;

        ToolsPluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<ToolsPluginInterface>(QString::fromLatin1("Rocs/ToolPlugin"), QString::fromLatin1("[Name]=='%1'").arg(name), this, QVariantList(), &error);

        if (plugin) {
            _d->toolsPluginsMap.insert(kpi, plugin);
            kpi.setPluginEnabled(true);
            return true;
        }

        else {
            kWarning() << "error loading plugin: " << name << error;
        }

    } else {
        kWarning() << "Error loading tool plugin " << name;
    }
    return false;
}


void ToolManager::loadToolsPlugins()
{
    foreach(KPluginInfo info, _d->toolsPluginsInfo) {
        loadToolPlugin(info.name());
    }
}


KPluginInfo ToolManager::pluginInfo(ToolsPluginInterface* plugin)
{
    return _d->toolsPluginsMap.key(plugin);
}


QList< ToolsPluginInterface* > ToolManager::toolPlugins()
{
    loadToolsPlugins();
    QList < ToolsPluginInterface * > value;

    QMap<KPluginInfo, ToolsPluginInterface*>::const_iterator iter;
    for (iter = _d->toolsPluginsMap.constBegin(); iter != _d->toolsPluginsMap.constEnd(); ++iter) {
        if (iter.key().isPluginEnabled()){
            value.append(_d->toolsPluginsMap[iter.key()]);
        }
    }
    return value;
}


KPluginInfo pluginInfo(const ToolsPluginInterface * /*plugin*/)
{
    return KPluginInfo();
}
