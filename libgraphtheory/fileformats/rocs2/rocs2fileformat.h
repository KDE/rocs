/*
 *  SPDX-FileCopyrightText: 2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
