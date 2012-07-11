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

#ifndef FILEPLUGININTERFACE_H
#define FILEPLUGININTERFACE_H
#include <QObject>
#include "rocslib_export.h"

class KUrl;
class Document;
class FilePluginInterfacePrivate;

#include <KComponentData>

class ROCSLIB_EXPORT FilePluginInterface: public QObject
{
    Q_OBJECT

public:
    enum Error {
        None,
        FileIsReadOnly
    };

    FilePluginInterface(const KComponentData &instance, QObject* parent);
    virtual ~FilePluginInterface();

    void setFile(const KUrl& file);

    /**
     * Return true if last read or write operation caused an error.
     */
    bool hasError() const;

    /**
     * Returns last error
     */
    virtual Error error() const;

    /**
     * Return explenation of last error in human readable text.
     */
    virtual QString errorString() = 0;

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

    virtual Document* graphDocument();


private:
    FilePluginInterfacePrivate *d;
};

#endif // FILEPLUGININTERFACE_H
