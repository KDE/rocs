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

#include "GraphFilePluginInterface.h"

#include <QStringList>
#include <QObject>
#include <Document.h>
#include <QUrl>
#include <QDebug>
#include <KAboutData>

class GraphFilePluginInterfacePrivate
{
public:
    GraphFilePluginInterfacePrivate(const QString &componentName)
        : componentName(componentName)
        , aboutData(0) //FIXME
    {
        lastError = GraphFilePluginInterface::None;
    }

    const QString componentName;
    const KAboutData* aboutData; //FIXME value never set
    GraphFilePluginInterface::Error lastError;
    QString lastErrorString;
    Document* graphDocument;
    QUrl file;
};


GraphFilePluginInterface::GraphFilePluginInterface(const QString &componentName, QObject* parent):
    QObject(parent),
    d(new GraphFilePluginInterfacePrivate(componentName))
{
}


GraphFilePluginInterface::~GraphFilePluginInterface()
{
}


GraphFilePluginInterface::PluginType GraphFilePluginInterface::pluginCapability() const
{
    return ImportAndExport;
}


bool GraphFilePluginInterface::hasError() const
{
    return d->lastError != GraphFilePluginInterface::None;
}


GraphFilePluginInterface::Error GraphFilePluginInterface::error() const
{
    return d->lastError;
}


QString GraphFilePluginInterface::errorString() const
{
    return d->lastErrorString;
}


void GraphFilePluginInterface::setError(GraphFilePluginInterface::Error error, QString message)
{
    d->lastError = error;
    d->lastErrorString = message;
}


const KAboutData* GraphFilePluginInterface::aboutData() const
{
    return d->aboutData;
}


bool GraphFilePluginInterface::isGraphDocument() const
{
    return (d->graphDocument && d->graphDocument->dataStructures().count() > 0);
}


Document* GraphFilePluginInterface::graphDocument() const
{
    return d->graphDocument;
}


void GraphFilePluginInterface::setGraphDocument(Document* document)
{
    d->graphDocument = document;
}


void GraphFilePluginInterface::setFile(const QUrl& file)
{
    d->file = file;
}


const QUrl& GraphFilePluginInterface::file() const
{
    return d->file;
}

