/*
 *  Copyright 2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) version 3, or any
 *  later version accepted by the membership of KDE e.V. (or its
 *  successor approved by the membership of KDE e.V.), which shall
 *  act as a proxy defined in Section 6 of version 3 of the license.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ROCS2FILEFORMAT_H
#define ROCS2FILEFORMAT_H

#include "fileformats/fileformatinterface.h"
#include <edgetype.h>

namespace GraphTheory
{


/** \brief the Rocs new generation graph file format
 */
class Rocs2FileFormat : public FileFormatInterface
{
    Q_OBJECT
public:
    explicit Rocs2FileFormat(QObject *parent, const QList< QVariant >&);
    ~Rocs2FileFormat();

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
    QString direction(EdgeType::Direction direction) const;
    EdgeType::Direction direction(QString direction) const;
};
}

#endif
