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

#ifndef KMLFILEPLUGIN_H
#define KMLFILEPLUGIN_H
#include "../FilePluginInterface.h"

class PlainTXTFilePlugin: public Rocs::FilePluginInterface
{
  Q_OBJECT
  public:
    explicit PlainTXTFilePlugin(QObject* parent, const QList< QVariant >&);
    ~PlainTXTFilePlugin();

    const QStringList extensions() const; //Extensões suportadas

    GraphDocument * readFile(const QString& fileName) ; //return 0 se arq. inválido

    bool writeFile(GraphDocument& graph, const QString& filename) ; //false se não gravou.


    virtual const QString lastError();

    private:
        QString _lastError;
        void setError ( QString arg1 );
};

#endif // KMLFILEPLUGIN_H
