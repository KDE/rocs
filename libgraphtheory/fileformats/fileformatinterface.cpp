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

#include "fileformats/fileformatinterface.h"

#include "typenames.h"
#include <QStringList>
#include <QObject>
#include <QUrl>
#include <QDebug>

using namespace GraphTheory;

class GraphTheory::FileFormatInterfacePrivate
{
public:
    FileFormatInterfacePrivate(const QString &componentName)
        : componentName(componentName)
    {
        lastError = FileFormatInterface::None;
    }

    const QString componentName;
    FileFormatInterface::Error lastError;
    QString lastErrorString;
    GraphDocumentPtr graphDocument;
    QUrl file;
};


FileFormatInterface::FileFormatInterface(const QString &componentName, QObject *parent):
    QObject(parent),
    d(new FileFormatInterfacePrivate(componentName))
{
}

FileFormatInterface::~FileFormatInterface()
{
}

FileFormatInterface::PluginType FileFormatInterface::pluginCapability() const
{
    return ImportAndExport;
}

bool FileFormatInterface::hasError() const
{
    return d->lastError != FileFormatInterface::None;
}

FileFormatInterface::Error FileFormatInterface::error() const
{
    return d->lastError;
}

QString FileFormatInterface::errorString() const
{
    return d->lastErrorString;
}

void FileFormatInterface::setError(FileFormatInterface::Error error, const QString &message)
{
    d->lastError = error;
    d->lastErrorString = message;
}

bool FileFormatInterface::isGraphDocument() const
{
    return !d->graphDocument.isNull();
}

GraphDocumentPtr FileFormatInterface::graphDocument() const
{
    return d->graphDocument;
}

void FileFormatInterface::setGraphDocument(GraphDocumentPtr document)
{
    d->graphDocument = document;
}

void FileFormatInterface::setFile(const QUrl &file)
{
    d->file = file;
}

const QUrl& FileFormatInterface::file() const
{
    return d->file;
}
