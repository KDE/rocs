/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
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
};
}

#endif
