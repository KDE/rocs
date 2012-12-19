/*
    This file is part of Rocs.
    Copyright 2011  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef DOTFILEPLUGIN_H
#define DOTFILEPLUGIN_H

#include <QObject>

#include "FilePluginInterface.h"
#include "Rocs_Typedefs.h"

class Document;

class DotFilePlugin: public FilePluginInterface
{
    Q_OBJECT
public:

    explicit DotFilePlugin(QObject* parent, const QList< QVariant >&);
    ~DotFilePlugin();

    /** \brief read file given by the specified fileName
     * \param QString reference specifying the filename
     * \return Document pointer
     */
    Document* readFile(const QString& fileName);

    Document* parseGraphvizUndirected(const QString& graphvizContent);

    Document* parseGraphvizDirected(const QString& graphvizContent);

    /** \brief write specified data structure document to file
     * \param Document  the document containing list of data structures
     * \param QString  filename of the file the graph shall be serialized to
     * \return false if unsuccessfull, true if graph was read
     */
    bool writeFile(Document& document, const QString& filename);

    /** \brief returns list of supported extensions for this file plugin
     * \return QStringList
     */
    const QStringList extensions() const;

    virtual const QString lastError();

private:
    QString _lastError;
    void setError(QString arg);
    QString const processNode(DataPtr data) const;
    QString const processEdge(PointerPtr edge) const;
};

#endif // DOTFILEPLUGIN_H
