/*
    This file is part of Rocs.
    SPDX-FileCopyrightText: 2010 Tomaz Canabrava <tomaz.canabrava@gmail.com>
    SPDX-FileCopyrightText: 2010 Wagner Reck <wagner.reck@gmail.com>
    SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef DOTFILEFORMAT_H
#define DOTFILEFORMAT_H

#include "fileformats/fileformatinterface.h"
#include <QObject>

namespace GraphTheory
{

class DotFileFormat : public FileFormatInterface
{
    Q_OBJECT
public:
    explicit DotFileFormat(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    ~DotFileFormat() override;

    /**
     * File extensions that are common for this file type.
     */
    const QStringList extensions() const override;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graphDocument to be serialized
     */
    void writeFile(GraphDocumentPtr graph) override;

    /**
     * Open given file and imports it into internal format.
     * \param file is url of a local file
     */
    void readFile() override;
};
}

#endif
