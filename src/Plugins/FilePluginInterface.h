
class Private;/*
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

#ifndef FILEPLUGININTERFACE_H
#define FILEPLUGININTERFACE_H
#include <QObject>
#include "rocslib_export.h"

class GraphDocument;

#include <KComponentData>

namespace Rocs{
class ROCSLIB_EXPORT FilePluginInterface: public QObject
{
  Q_OBJECT
public:
  FilePluginInterface(const KComponentData &instance, QObject* parent); 
  virtual ~FilePluginInterface();
  virtual const QStringList extensions() const = 0; //Extensões suportadas
  
  virtual GraphDocument * readFile(const QString &file) const = 0; //return 0 se arq. inválido
  
  virtual bool writeFile(const GraphDocument &graph, const QString & fileName)  const = 0; //false se não gravou.
  
  const QString lastError(); //return error
  
private:
  class Private;
  Private * d;
};
}
#endif // FILEPLUGININTERFACE_H
