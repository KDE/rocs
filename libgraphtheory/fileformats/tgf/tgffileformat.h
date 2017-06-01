/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
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
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TGFFILEFORMAT_H
#define TGFFILEFORMAT_H

#include "fileformats/fileformatinterface.h"

namespace GraphTheory
{


/** \brief class TgfFileFormatPlugin: Import and Export Plugin for TGF
 *
 * This plugin class allows reading and writing of Trivial Graph Format (TGF) files.
 * TGF is a simple file format that can store directed graphs by a list of nodes and list of edges.
 * Each node and each edge can have exactly one label.
 *
 * Format Specification:
 *  - The file starts with a list of nodes (one node per line), followed by a line with the only
 *    character "#", followed by a list of edges (one edge per line).
 *  - A node consists of an integer (identifier), followed by a space, followed by an arbitrary string.
 *  - An edge consists of two integers (identifiers) separated by a space, followed by a space,
 *    followed by an arbitray string. It is assumed that the directed edge points from the first
 *    identifier to the second identifier.
 */

class TgfFileFormat : public FileFormatInterface
{
    Q_OBJECT
public:
    explicit TgfFileFormat(QObject *parent, const QList< QVariant >&);
    ~TgfFileFormat();

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
    enum ReadMode {
        Nodes,
        Edges
    };
};
}

#endif
