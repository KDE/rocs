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

#include "ToolManager.h"

#include "ToolsPluginInterface.h"

#include <KServiceTypeTrader>
#include <KPluginInfo>
#include <QDebug>
#include <KAboutData>

class ToolManagerPrivate
{
public:
    typedef KPluginInfo::List KPluginList;

    QObject *_parent;
    bool _initialized;
    KPluginList _toolsPluginsInfo;
    QMap<KPluginInfo, ToolsPluginInterface*> _toolsPluginsMap;

    ToolManagerPrivate(QObject *parent)
        : _parent(parent)
        , _initialized(false)
    {
        _toolsPluginsInfo = KPluginInfo::fromServices(KServiceTypeTrader::self()->query("Rocs/ToolPlugin"));
    }

    ~ToolManagerPrivate()
    {
    }

    void lazyInit()
    {
        if (!_initialized) {
            _initialized = true;
            loadPlugins();
        }
    }

    bool loadPlugin(QString name)
    {
        KPluginInfo kpi = PluginInfoFromName(name);

        if (kpi.isValid()) {
            QString error;
            ToolsPluginInterface * plugin = KServiceTypeTrader::createInstanceFromQuery<ToolsPluginInterface>(
                                                    QString::fromLatin1("Rocs/ToolPlugin"),
                                                    QString::fromLatin1("[Name]=='%1'").arg(name),
                                                    _parent,
                                                    QVariantList(),
                                                    &error);

            if (plugin) {
                _toolsPluginsMap.insert(kpi, plugin);
                kpi.setPluginEnabled(true);
                return true;
            }

            else {
                qWarning() << "Error while loading plugin: " << name << error;
            }

        } else {
            qWarning() << "Error while loading tool plugin " << name;
        }
        return false;
    }

    void loadPlugins()
    {
        foreach(const KPluginInfo &info, _toolsPluginsInfo) {
            loadPlugin(info.name());
        }
    }

    KPluginInfo PluginInfoFromName(const QString &name)
    {
        foreach(const KPluginInfo &info, _toolsPluginsInfo) {
            if (info.name() == name) {
                return info;
            }
        }
        return KPluginInfo();
    }
};

ToolManager & ToolManager::self()
{
    static ToolManager instance;
    instance.d->lazyInit();
    return instance;
}


ToolManager::ToolManager()
    : d(new ToolManagerPrivate(this))
{
}


ToolManager::~ToolManager()
{
}


KPluginInfo ToolManager::pluginInfo(ToolsPluginInterface *plugin)
{
    if (d->_toolsPluginsMap.keys(plugin).length() == 0) {
        d->loadPlugin(plugin->displayName());
    }

    return d->_toolsPluginsMap.key(plugin);
}


QList<ToolsPluginInterface *> ToolManager::plugins() const
{
    QList<ToolsPluginInterface *> plugins;

    QMap<KPluginInfo, ToolsPluginInterface *>::const_iterator iter;
    for (iter = d->_toolsPluginsMap.constBegin(); iter != d->_toolsPluginsMap.constEnd(); ++iter) {
        if (iter.key().isPluginEnabled()){
            plugins.append(d->_toolsPluginsMap[iter.key()]);
        }
    }
    return plugins;
}
