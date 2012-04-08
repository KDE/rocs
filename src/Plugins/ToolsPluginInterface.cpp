/*
    This file is part of Rocs.
    Copyright 2001-2002  Duncan Mac-Vicar Prett <duncan@kde.org>
    Copyright 2002-2004  Olivier Goffart        <ogoffart@kde.org>
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ToolsPluginInterface.h"


#include <kplugininfo.h>
#include "PluginManager.h"


class  ToolsPluginInterface::Private
{
public:
    KPluginInfo *_pluginInfo;
    QStringList addressBookFields;
    QString indexField;
};

ToolsPluginInterface::ToolsPluginInterface(const KComponentData& instance, QObject* parent)
    : QObject(parent), KXMLGUIClient(), d(new Private)
{
    setComponentData(instance);

}

ToolsPluginInterface::~ToolsPluginInterface()
{
    delete d;
}

QString ToolsPluginInterface::pluginId()
{
    return QString::fromLatin1(metaObject()->className());
}


QString ToolsPluginInterface::displayName()
{
    return pluginInfo().isValid() ? pluginInfo().name() : QString();
}

QString ToolsPluginInterface::pluginIcon()
{
    return pluginInfo().isValid() ? pluginInfo().icon() : QString();
}


KPluginInfo ToolsPluginInterface::pluginInfo()
{
    return PluginManager::instance()->pluginInfo(this);
//  return *(d->_pluginInfo);
}
