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

#include "FilePluginInterface.h"

#include <QStringList>
#include <QObject>
#include <Document.h>
#include <KUrl>

class FilePluginInterfacePrivate
{
public:
    FilePluginInterfacePrivate() {
        lastError = FilePluginInterface::None;
    }

    FilePluginInterface::Error lastError;
    Document* graphDocument;
    KUrl file;
};


FilePluginInterface::FilePluginInterface(const KComponentData& /*instance*/, QObject* parent):
    QObject(parent)
{

}


FilePluginInterface::~FilePluginInterface()
{

}


bool FilePluginInterface::hasError() const
{
    return d->lastError != FilePluginInterface::None;
}


FilePluginInterface::Error FilePluginInterface::error() const
{
    return d->lastError;
}


bool FilePluginInterface::isGraphDocument() const
{
    return (d->graphDocument && d->graphDocument->dataStructures().count() > 0);
}


Document* FilePluginInterface::graphDocument()
{
    return d->graphDocument;
}


void FilePluginInterface::setFile(const KUrl& file)
{
    d->file = file;
}
