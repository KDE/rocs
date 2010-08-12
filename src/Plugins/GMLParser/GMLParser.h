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

#ifndef DOTPARSER_H
#define DOTPARSER_H

#include <QObject>

#include <Plugins/FilePluginInterface.h>
class Edge;
class Node;

class GMLParser: public Rocs::FilePluginInterface
{
  Q_OBJECT
  public:
    explicit GMLParser(QObject* parent, const QList< QVariant >&);
    ~GMLParser();

    const QStringList extensions() const; //Extensões suportadas

    GraphDocument * readFile(const QString& fileName) ; //return 0 se arq. inválido

    bool writeFile(GraphDocument& graph, const QString& filename) ; //false se não gravou.


    virtual const QString lastError();

    virtual const QString scriptToRun();

    private:
        QString _lastError;
        void setError ( QString arg1 );
        QString const processNode(Node*) const;
        QString const processEdge(Edge* e) const;

};

#endif // DOTPARSER_H
