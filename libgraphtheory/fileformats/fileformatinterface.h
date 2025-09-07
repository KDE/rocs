/*
 *  SPDX-FileCopyrightText: 2012-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#ifndef FILEFORMATINTERFACE_H
#define FILEFORMATINTERFACE_H

#include "graphtheory_export.h"
#include "typenames.h"
#include <QObject>

class QUrl;

namespace GraphTheory
{

class FileFormatInterfacePrivate;

/**
 * \class FileFormatInterface
 * This class provides an interface for graph file format plugins.
 * A graph file format plugin must provide implementations of
 * - writeFile(...)
 * - readFile()
 * - extensions()
 * and optionally, if it is not a read and write plugin, an implementation of
 * - pluginCapability()
 * to specify the plugin capabilities.
 */
class GRAPHTHEORY_EXPORT FileFormatInterface
{
public:
    /**
     * Describes the last error of the plugin.
     */
    enum Error { None, Unknown, FileIsReadOnly, CouldNotOpenFile, NoGraphFound, EncodingProblem, CouldNotRecognizeFileFormat, NotSupportedOperation };

    /**
     * Describes the capability of the plugin, i.e., if the plugin can be used
     * to read and/or write files.
     */
    enum PluginType { ImportOnly, ExportOnly, ImportAndExport };

    /**
     * Constructor
     */
    FileFormatInterface();
    virtual ~FileFormatInterface();

    /**
     * Returns \p PluginType to indicate whether the plugin only provides import, only export
     * or both capabilities. This method should be used to determine in which file dialogs
     * the plugin shall be included.
     *
     * \return is by default PluginType::ImportAndExport
     */
    virtual PluginType pluginCapability() const;

    /**
     * \return true if last read or write operation caused an error.
     */
    bool hasError() const;

    /**
     * Returns last error. If last operation was successful.
     * Use \see hasError() to test if error exists.
     *
     * \return last Error of the plugin.
     */
    Error error() const;

    /**
     * \return last error as human readable text
     */
    QString errorString() const;

    /**
     * File extensions that are common for this file type.
     *
     * \return extension list
     */
    virtual const QStringList extensions() const = 0;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     *
     * \param document is graph document to be serialized
     */
    virtual void writeFile(GraphDocumentPtr document) = 0;

    /**
     * Open given file and imports it into internal format.
     */
    virtual void readFile() = 0;

    /**
     * Set file that shall be used for nexte read or write operation.
     *
     * \param file is QUrl pointing to local file
     */
    void setFile(const QUrl &file);

    /**
     * \return true if a valid graph document was read. Otherwise return false
     */
    virtual bool isGraphDocument() const;

    /**
     * If \see isGraphDocument() returns true, this method returns document created by last
     * \see readFile() call.
     *
     * \return the graph document
     */
    virtual GraphDocumentPtr graphDocument() const;

protected:
    /**
     * \internal
     * Gives current file.
     *
     * \return file that is last used for write/read
     */
    const QUrl &file() const;

    /**
     * \internal
     * Use this function for import-plugins to set the opened graph document and setting the modified state to false.
     *
     * \param document that contains the graph
     */
    void setGraphDocument(GraphDocumentPtr document);

    /**
     * \internal
     * Use this function to set or unset the current error of the plugin. To unset an error
     * use Error::None.
     *
     * \param error is type of error, \see FileFormatInterface::Error
     * \param message is optional description for error
     */
    void setError(Error error, const QString &message = QString());

private:
    const std::unique_ptr<FileFormatInterfacePrivate> d;
};
}

#endif
