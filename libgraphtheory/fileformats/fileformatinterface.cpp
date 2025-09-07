/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include <QStringList>
#include <QUrl>

using namespace GraphTheory;

class GraphTheory::FileFormatInterfacePrivate
{
public:
    FileFormatInterface::Error lastError = FileFormatInterface::None;
    QString lastErrorString;
    GraphDocumentPtr graphDocument;
    QUrl file;
};

FileFormatInterface::FileFormatInterface()
    : d(std::make_unique<FileFormatInterfacePrivate>())
{
}

FileFormatInterface::~FileFormatInterface() = default;

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

const QUrl &FileFormatInterface::file() const
{
    return d->file;
}

#include "moc_fileformatinterface.cpp"
