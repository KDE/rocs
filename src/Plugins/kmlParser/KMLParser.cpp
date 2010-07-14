/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "KMLParser.h"
#include "Core/graphDocument.h"
#include <KAboutData>
#include <KGenericFactory>
#include <QFile>
#include <KDebug>
#include <Core/graph.h>
#include <Core/node.h>
#include <QXmlQuery>
#include <QXmlResultItems>
#include <QXmlNodeModelIndex>
#include "KMLHandler.h"

static const KAboutData aboutdata ( "rocs_kmlplugin", 0, ki18n ( "Open and Save Keyhole Markup Language files" ) , "0.1" );


K_PLUGIN_FACTORY ( FilePLuginFactory, registerPlugin< Rocs::KMLParser >(); )
K_EXPORT_PLUGIN ( FilePLuginFactory ( aboutdata ) )


namespace Rocs {
KMLParser::KMLParser ( QObject* parent, const QList< QVariant >& ) :
        FilePluginInterface ( FilePLuginFactory::componentData(), parent ) {

}


bool KMLParser::writeFile(GraphDocument& graph, const QString& fileName)
{
    QFile file(fileName);
    Graph * g = graph.activeGraph();
    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        _lastError = i18n("Cannot open file %1: %2").arg(fileName).arg(file.errorString());
        return false;
    }
    QXmlStreamWriter xmlWriter(&file);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("kml");
    xmlWriter.writeNamespace("http://www.opengis.net/kml/2.2");
    xmlWriter.writeStartElement("Document");
    if (g->edges().isEmpty()) {
        foreach(Node * n, g->nodes()) {
            xmlWriter.writeStartElement("Placemark");
            xmlWriter.writeStartElement("name");
            xmlWriter.writeCharacters(n->name());
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
            QString s = g->nodes().at(0)->name();
            s.chop(2);
            xmlWriter.writeCharacters(s);
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("description");

        if (g->nodes().at(0)->property("description").isValid()) {
            xmlWriter.writeCharacters(g->nodes().at(0)->property("description").toString());
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("LineString");
        xmlWriter.writeStartElement("coordinates");

        foreach (Node* n, g->nodes()) {
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
    return true;
}

GraphDocument* KMLParser::readFile(const QString& file)
{
    GraphDocument * graphDoc = new GraphDocument("KML File");
    Graph * g = graphDoc->addGraph();

    KMLHandler handler(g);
    QFile f(file);
    QXmlInputSource source(&f);
    QXmlSimpleReader sr;
    sr.setContentHandler(&handler);
    sr.setErrorHandler(&handler);
    if (!sr.parse(&source)) {
        _lastError = handler.errorString();
        delete graphDoc;
        return 0;
    }

    return graphDoc;
}

const QStringList KMLParser::extensions() const
{
    return QStringList()
           << i18n ( "*.kml|Keyhole Markup Language Files" ) + '\n';
}
const QString KMLParser::lastError()
{
    return _lastError;
}

const QString KMLParser::scriptToRun()
{
    return Rocs::FilePluginInterface::scriptToRun();
}

}
#include "KMLParser.moc"