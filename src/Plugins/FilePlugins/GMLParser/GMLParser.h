/*
    This file is part of Rocs.
    Copyright 2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef GMLPARSER_H
#define GMLPARSER_H

#include <QObject>

#include "FilePluginInterface.h"
#include "Rocs_Typedefs.h"
class Pointer;
class Data;

class GMLParser: public FilePluginInterface
{
  Q_OBJECT
  public:
    explicit GMLParser(QObject* parent, const QList< QVariant >&);
    ~GMLParser();

    const QStringList extensions() const; //Extensões suportadas

    Document * readFile(const QString& fileName) ; //return 0 se arq. inválido

    bool writeFile(Document& graph, const QString& filename) ; //false se não gravou.


    virtual const QString lastError();

    virtual const QString scriptToRun();

    private:
        QString _lastError;
        void setError ( QString arg1 );
        QString const processNode(DataPtr) const;
        QString const processEdge(PointerPtr e) const;

};

#endif // GMLPARSER_H
