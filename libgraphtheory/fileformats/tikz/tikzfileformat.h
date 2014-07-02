/*
    This file is part of Rocs.
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

#ifndef TIKZFILEFORMAT_H
#define TIKZFILEFORMAT_H

#include "fileformats/fileformatinterface.h"

namespace GraphTheory
{

/** \brief class TikzFileFormatPlugin: Export Plugin for PGF/TikZ files
 *
 * This plugin class allows (only) writing of PGF/TikZ files for usage in LaTeX documents.
 */

class TikzFileFormat : public FileFormatInterface
{
    Q_OBJECT
public:
    explicit TikzFileFormat(QObject* parent, const QList< QVariant >&);
    ~TikzFileFormat();

    virtual FileFormatInterface::PluginType pluginCapability() const;

    /**
     * File extensions that are common for this file type.
     */
    virtual const QStringList extensions() const;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graph document to be serialized
     */
    virtual void writeFile(GraphDocumentPtr graph);

    /**
     * Open given file and imports it into internal format.
     */
    virtual void readFile();
};
}

#endif
