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

#include "KMLHandler.h"
#include <KDebug>


KMLHandler::KMLHandler(DataStructureBase* doc): QXmlDefaultHandler(), m_graph(doc)
{

}


KMLHandler::~KMLHandler()
{

}

bool KMLHandler::characters(const QString& str)
{
    currentText += str;
    return true;
}

bool KMLHandler::endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
{
  Node * n = 0;
    if (qName == "coordinates") {
        m_coordinates = currentText;
    } else if (qName == "name") {
        m_name = currentText;
    } else if (qName == "description") {
        m_description = currentText;
    } else if (qName == "Placemark"  && !m_coordinates.isEmpty()) {
        if (m_name.isEmpty()) {
            m_name = QString::number(qrand());
        }
        n = m_graph->addNode(m_name);
        QStringList values = m_coordinates.split(',');
        if (values.count() >=2) {

            n->setX(values.at(0).toDouble());
            n->addDynamicProperty("Longitude", values.at(0).toDouble());
            n->setY(values.at(1).toDouble());
            n->addDynamicProperty("Latitude", values.at(1).toDouble());
            if (values.count() >2) {
                n->addDynamicProperty("Elevation", values.at(2).toDouble());
            }
        }
        if (!m_description.isEmpty()) {
            n->addDynamicProperty("Description", m_description);
        }
        m_coordinates.clear();
        m_description.clear();
        m_name.clear();
    }
    if (qName == "LineString" && !m_coordinates.isEmpty()) {
        QStringList points = m_coordinates.split('\n');
        int count = 0;
        Node * n_old = 0;
        foreach (QString point, points) {
            count++;

            QStringList values = point.split(',');
            if (values.count() >=2) {
               n = m_graph->addNode(m_name + "_" + QString::number(count+1));

                n->setX(values.at(0).toDouble());
                n->addDynamicProperty("Longitude", values.at(0).toDouble());
                n->setY(values.at(1).toDouble());
                n->addDynamicProperty("Latitude", values.at(1).toDouble());
                if (values.count() >2) {
                    n->addDynamicProperty("Elevation", values.at(2).toDouble());
                }
            }
            if (!m_description.isEmpty()) {
                n->addDynamicProperty("Description", m_description);
            }
            if (n_old != 0){
              m_graph->addEdge(n_old, n);
            }
            n_old = n;
        }
        m_coordinates.clear();
        m_description.clear();
        m_name.clear();
    }
    kDebug() << qName;
    return true;
}


QString KMLHandler::errorString() const
{
    return QXmlDefaultHandler::errorString();
}


bool KMLHandler::fatalError(const QXmlParseException& exception)
{
    return QXmlDefaultHandler::fatalError(exception);
}



bool KMLHandler::startElement(const QString& namespaceURI, const QString& localName, const QString& qName, const QXmlAttributes& attributes)
{
    currentText.clear();
    return true;

}
