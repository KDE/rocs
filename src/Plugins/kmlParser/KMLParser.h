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

#ifndef KMLPARSER_H
#define KMLPARSER_H

#include <Plugins/FilePluginInterface.h>

namespace Rocs{
  class KMLParser : public FilePluginInterface
  {

  public:
      KMLParser(QObject* parent, const QList< QVariant >& opts);
      virtual bool writeFile(GraphDocument& graph, const QString& fileName);
      virtual GraphDocument* readFile(const QString& file);
      virtual const QStringList extensions() const;
      virtual const QString scriptToRun();
      virtual const QString lastError();

private:
QString _lastError;

  };
}
#endif // KMLPARSER_H
