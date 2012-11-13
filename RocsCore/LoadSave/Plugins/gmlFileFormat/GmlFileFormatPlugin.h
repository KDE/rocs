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

#ifndef GMLFILEFORMATPLUGIN_H
#define GMLFILEFORMATPLUGIN_H

#include <QObject>

#include "GraphFilePluginInterface.h"
#include "CoreTypes.h"
class Pointer;
class Data;

class GmlFileFormatPlugin: public GraphFilePluginInterface
{
    Q_OBJECT
public:
    explicit GmlFileFormatPlugin(QObject *parent, const QList< QVariant >&);
    ~GmlFileFormatPlugin();

    /**
     * File extensions that are common for this file type.
     */
    virtual const QStringList extensions() const;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graphDocument to be serialized
     */
    virtual void writeFile(Document &document);

    /**
     * Open given file and imports it into internal format.
     */
    virtual void readFile();

private:
    QString const processNode(DataPtr node) const;
    QString const processEdge(PointerPtr edge) const;
};

#endif // GMLFILEFORMATPLUGIN_H
