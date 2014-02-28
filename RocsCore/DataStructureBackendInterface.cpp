/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#include "DataStructureBackendInterface.h"
#include "DataStructureBackendManager.h"
#include <KPluginInfo>
#include <KXMLGUIClient>
#include <QString>

DataStructureBackendInterface::DataStructureBackendInterface(const QString &componentName, QObject* parent)
    : QObject(parent)
{
    setComponentName(componentName, componentName);
}

DataStructureBackendInterface::~DataStructureBackendInterface()
{
}


QLayout * DataStructureBackendInterface::pointerExtraProperties(PointerPtr arg1, QWidget* arg2) const
{
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout * DataStructureBackendInterface::dataStructureExtraProperties(DataStructurePtr arg1, QWidget* arg2) const
{
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout * DataStructureBackendInterface::dataExtraProperties(DataPtr arg1, QWidget* arg2) const
{
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}


QString DataStructureBackendInterface::name()
{
    if (DataStructureBackendManager::self().backendInfo(this).isValid()) {
        return DataStructureBackendManager::self().backendInfo(this).name();
    }
    return QString();
}


QString DataStructureBackendInterface::internalName()
{
    KPluginInfo pluginInfo = DataStructureBackendManager::self().backendInfo(this);
    if (pluginInfo.isValid()) {
        return pluginInfo.property(QLatin1String("X-Rocs-DataStructureIdentifier")).toString();
    }
    return QString();
}
