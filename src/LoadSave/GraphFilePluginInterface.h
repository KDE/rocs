/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef GRAPHFILEPLUGININTERFACE_H
#define GRAPHFILEPLUGININTERFACE_H
#include <QObject>
#include "rocslib_export.h"

class KUrl;
class Document;
class GraphFilePluginInterfacePrivate;

#include <KComponentData>

class ROCSLIB_EXPORT GraphFilePluginInterface: public QObject
{
    Q_OBJECT

public:
    enum Error {
        None,
        Unknown,
        FileIsReadOnly,
        CouldNotOpenFile,
        NoGraphFound,
        CouldNotRecognizeFileFormat
    };

    enum PluginType {
        ImportOnly,
        ExportOnly,
        ImportAndExport
    };

    GraphFilePluginInterface(const KComponentData &instance, QObject* parent);
    virtual ~GraphFilePluginInterface();

    void setFile(const KUrl& file);

    /**
     * Returns \p PluginType to indicate wether the plugin only provides import, only export
     * or both capabilities. This method should be used to determine in which file dialogs
     * the plugin shall be included.
     * \return is by default PluginType::ImportAndExport
     */
    virtual PluginType pluginCapability() const;

    /**
     * Return true if last read or write operation caused an error.
     */
    bool hasError() const;

    /**
     * Returns last error
     */
    Error error() const;

    /**
     * Return explenation of last error in human readable text.
     */
    QString errorString() const;

    /**
     * File extensions that are common for this file type.
     */
    virtual const QStringList extensions() const = 0;

    /**
     * Writes given graph document to formerly specified file \see setFile().
     * \param graph is graphDocument to be serialized
     */
    virtual void writeFile(Document &graph) = 0;

    /**
     * Open given file and imports it into internal format.
     * \param file is url of a local file
     */
    virtual void readFile() = 0;

    /**
     * Return true if a valid graph document was read. Otherwise return false.
     */
    virtual bool isGraphDocument() const;

    virtual Document* graphDocument() const;


protected:
    /**
     * Return file that is used for write/read.
     */
    const KUrl& file() const;

    /**
     * Set graph document.
     */
    void setGraphDocument(Document* document);

    /**
     * Set last error.
     * \param error is type of error, \see GraphFilePluginInterface::Error
     * \param message is optional description for error
     */
    void setError(Error error, QString message=QString());

private:

    GraphFilePluginInterfacePrivate *d;
};

#endif // FILEPLUGININTERFACE_H
