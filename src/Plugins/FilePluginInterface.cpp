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

#include "FilePluginInterface.h"

#include <QStringList>
#include <QObject>

FilePluginInterface::FilePluginInterface(const KComponentData& /*instance*/, QObject* parent):
    QObject(parent)
{

}


FilePluginInterface::~FilePluginInterface()
{

}

const QString FilePluginInterface::lastError()
{
    return QString("Default Error.");
}
