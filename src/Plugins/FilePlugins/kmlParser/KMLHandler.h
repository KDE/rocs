/*
    This file is part of Rocs.
    Copyright 2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

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


#ifndef KMLHANDLER_H
#define KMLHANDLER_H

#include <Core/DataStructure.h>
#include <QXmlDefaultHandler>


class KMLHandler: public QXmlDefaultHandler
{
  public:
  KMLHandler(DataStructurePtr doc);
  ~KMLHandler();


    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

//     QString method(){return this->m_method;}

    QString arg(int index){
        if (index < args.size()){
           return args.at(index);
        }
        return QString("");
    }

private:
    QString currentText;
    QString errorStr;

    QString m_name;
    DataStructurePtr m_graph;
    QStringList args;
    QString m_coordinates;
    QString m_description;

};

#endif // KMLHANDLER_H
