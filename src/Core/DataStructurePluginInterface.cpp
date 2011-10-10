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

#include "DataStructurePluginInterface.h"
#include "DataStructurePluginManager.h"

DataStructurePluginInterface::DataStructurePluginInterface ( const KComponentData& /*instance*/, QObject* parent )
: QObject ( parent )
{
}

DataStructurePluginInterface::~DataStructurePluginInterface()
{
}


QLayout* DataStructurePluginInterface::pointerExtraProperties ( PointerPtr arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* DataStructurePluginInterface::dataStructureExtraProperties ( DataStructurePtr arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}

QLayout* DataStructurePluginInterface::dataExtraProperties ( DataPtr arg1, QWidget* arg2 ) const {
    Q_UNUSED(arg1);
    Q_UNUSED(arg2);
    return 0;
}


QString DataStructurePluginInterface::name()
{
    if(DataStructurePluginManager::self()->pluginInfo(this).isValid()){
      return DataStructurePluginManager::self()->pluginInfo(this).name();
    }
    return QString();
}
