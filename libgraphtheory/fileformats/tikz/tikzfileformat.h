/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
    explicit TikzFileFormat(QObject *parent, const KPluginMetaData &data, const QVariantList &);
    ~TikzFileFormat() override;

    FileFormatInterface::PluginType pluginCapability() const override;

    /**
     * File extensions that are common for this file type.
     */
    const QStringList extensions() const override;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graph document to be serialized
     */
    void writeFile(GraphDocumentPtr graph) override;

    /**
     * Open given file and imports it into internal format.
     */
    void readFile() override;
};
}

#endif
