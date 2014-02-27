/*
    This file is part of Rocs.
    Copyright 2001-2002  Duncan Mac-Vicar Prett <duncan@kde.org>
    Copyright 2002-2004  Olivier Goffart        <ogoffart@kde.org>
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2013       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include "ToolManager.h"
#include <KPluginInfo>
#include <KDebug>
#include <KXMLGUIClient>

class ToolsPluginInterfacePrivate
{
public:
    // currently not needed
};


ToolsPluginInterface::ToolsPluginInterface(const QString &componentName, QObject *parent)
    : QObject(parent)
    , KXMLGUIClient()
    , d(new ToolsPluginInterfacePrivate)
{
    setComponentName(componentName, componentName);
}

ToolsPluginInterface::~ToolsPluginInterface()
{
    delete d;
}

QString ToolsPluginInterface::pluginId() const
{
    return QString::fromLatin1(metaObject()->className());
}


QString ToolsPluginInterface::displayName()
{
    return pluginInfo().isValid() ? pluginInfo().name() : QString();
}

QStringList ToolsPluginInterface::supportedDataStructures()
{
    return pluginInfo().property(QLatin1String("X-Rocs-SupportedDataStructures")).toStringList();
}

KPluginInfo ToolsPluginInterface::pluginInfo()
{
    return ToolManager::self().pluginInfo(this);
}
