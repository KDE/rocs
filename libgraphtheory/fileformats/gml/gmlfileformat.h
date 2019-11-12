/*
    This file is part of Rocs.
    Copyright 2010       Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GMLFILEFORMAT_H
#define GMLFILEFORMAT_H

#include <QObject>
#include "fileformats/fileformatinterface.h"

namespace GraphTheory
{

class GmlFileFormat : public FileFormatInterface
{
    Q_OBJECT
public:
    explicit GmlFileFormat(QObject *parent, const QList< QVariant >&);
    ~GmlFileFormat();

    /**
     * File extensions that are common for this file type.
     */
    const QStringList extensions() const Q_DECL_OVERRIDE;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graphDocument to be serialized
     */
    void writeFile(GraphDocumentPtr graph) Q_DECL_OVERRIDE;

    /**
     * Open given file and imports it into internal format.
     * \param file is url of a local file
     */
    void readFile() Q_DECL_OVERRIDE;

private:
    QString processNode(NodePtr node) const;
    QString processEdge(EdgePtr edge) const;
};
}

#endif
