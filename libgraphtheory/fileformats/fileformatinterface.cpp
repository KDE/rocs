/*
 *  Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "fileformats/fileformatinterface.h"

#include "graphdocument.h"
#include <QStringList>
#include <QUrl>

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
    document->setModified(false);
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
