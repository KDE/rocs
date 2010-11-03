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
#include <Core/DataType.h>
#include <Core/Data.h>
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


bool KMLParser::writeFile(DataTypeDocument& graph, const QString& fileName)
{
    QFile file(fileName);
    DataType * g = graph.activeDataType();
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
    if (g->pointers().isEmpty()) {
        foreach(Datum * n, g->data()) {
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
            QString s = g->data().at(0)->name();
            s.chop(2);
            xmlWriter.writeCharacters(s);
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("description");

        if (g->data().at(0)->property("description").isValid()) {
            xmlWriter.writeCharacters(g->data().at(0)->property("description").toString());
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("LineString");
        xmlWriter.writeStartElement("coordinates");

        foreach (Datum* n, g->data()) {
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

DataTypeDocument* KMLParser::readFile(const QString& file)
{
    DataTypeDocument * graphDoc = new DataTypeDocument("KML File");
    DataType * g = graphDoc->addDataType();

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

