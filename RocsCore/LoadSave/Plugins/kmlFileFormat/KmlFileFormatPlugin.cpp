/*
    This file is part of Rocs.
    Copyright 2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "KmlFileFormatPlugin.h"

#include "Document.h"
#include "DataStructure.h"
#include <DataStructureBackendManager.h>
#include "Data.h"
#include "KmlHandler.h"

#include <QDebug>
#include <KAboutData>
#include <KGenericFactory>

#include <QFile>


//FIXME
// static const KAboutData aboutdata("rocs_kmlfileformat",
//                                   0,
//                                   ki18nc("@title Displayed plugin name", "KML File Backend"),
//                                   "0.1",
//                                   ki18n("Read and write Keyhole Markup Language (KML) files."),
//                                   KAboutData::License_GPL_V2);

K_PLUGIN_FACTORY(FilePLuginFactory, registerPlugin<KmlFileFormatPlugin>();)

KmlFileFormatPlugin::KmlFileFormatPlugin(QObject *parent, const QList< QVariant >&)
    : GraphFilePluginInterface("rocs_kmlfileformat", parent)
{
}

KmlFileFormatPlugin::~KmlFileFormatPlugin()
{
}


const QStringList KmlFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.kml|Keyhole Markup Language Format") + '\n';
}


void KmlFileFormatPlugin::writeFile(Document& document)
{
    // TODO allow selection which data structure shall be exported
    QFile fileHandle(file().toLocalFile());
    DataStructurePtr graph = document.activeDataStructure();
    if (!fileHandle.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setError(FileIsReadOnly, i18n("Cannot open file %1: %2", file().fileName(), fileHandle.errorString()));
        return;
    }
    QXmlStreamWriter xmlWriter(&fileHandle);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("kml");
    xmlWriter.writeNamespace("http://www.opengis.net/kml/2.2");
    xmlWriter.writeStartElement("Document");
    //FIXME only default data type considered
    if (graph->pointers(0).isEmpty()) {
        foreach(DataPtr n, graph->dataList(0)) {
            xmlWriter.writeStartElement("Placemark");
            xmlWriter.writeStartElement("name");
            xmlWriter.writeCharacters(n->property("name").toString());
            if (n->property("description").isValid()) {
                xmlWriter.writeCharacters(n->property("description").toString());
            }
            xmlWriter.writeStartElement("Point");
            xmlWriter.writeStartElement("coordinates");

            if (n->property("Longitude").isValid()) {
                xmlWriter.writeCharacters(QString("%1,%2,%3").arg(n->property("Longitude").toString(),
                                          n->property("Latitude").toString(),
                                          n->property("Elevation").toString()));
            } else {
                xmlWriter.writeCharacters(QString("%1,%2,%3").arg(n->property("x").toString(),
                                          n->property("y").toString())
                                          .arg(0));
            }
        }
    } else {
        xmlWriter.writeStartElement("Placemark");
        xmlWriter.writeStartElement("name");
        {
            QString s = graph->dataList(0).at(0)->property("name").toString();
            s.chop(2);
            xmlWriter.writeCharacters(s);
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("description");

        if (graph->dataList(0).at(0)->property("description").isValid()) {
            xmlWriter.writeCharacters(graph->dataList(0).at(0)->property("description").toString());
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("LineString");
        xmlWriter.writeStartElement("coordinates");

        foreach(DataPtr n, graph->dataList(0)) {
            if (n->property("Longitude").isValid()) {
                xmlWriter.writeCharacters(QString("%1,%2,%3\n").arg(n->property("Longitude").toString(),
                                          n->property("Latitude").toString(),
                                          n->property("Elevation").toString()));
            } else {
                xmlWriter.writeCharacters(QString("%1,%2,%3").arg(n->property("x").toString(),
                                          n->property("y").toString())
                                          .arg(0));
            }
        }

    }
    xmlWriter.writeEndDocument();
    setError(None);
    return;
}


void KmlFileFormatPlugin::readFile()
{
    Document * graphDoc = new Document(i18n("Import"));
    DataStructureBackendManager::self().setBackend("Graph");
    DataStructurePtr graph = graphDoc->addDataStructure();

    KmlHandler handler(graph);
    QFile fileHandle(file().toLocalFile());
    QXmlInputSource source(&fileHandle);
    QXmlSimpleReader xmlReader;
    xmlReader.setContentHandler(&handler);
    xmlReader.setErrorHandler(&handler);
    if (!xmlReader.parse(&source)) {
        setError(EncodingProblem, handler.errorString());
        delete graphDoc;
        return;
    }

    setGraphDocument(graphDoc);
    return;
}

#include "KmlFileFormatPlugin.moc"

